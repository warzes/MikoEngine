#pragma once

namespace Rhi
{
	enum class CommandDispatchFunctionIndex : uint8_t
	{
		// Command buffer
		EXECUTE_COMMAND_BUFFER = 0,
		// Graphics
		SET_GRAPHICS_ROOT_SIGNATURE,
		SET_GRAPHICS_PIPELINE_STATE,
		SET_GRAPHICS_RESOURCE_GROUP,
		SET_GRAPHICS_VERTEX_ARRAY,			// Input-assembler (IA) stage
		SET_GRAPHICS_VIEWPORTS,				// Rasterizer (RS) stage
		SET_GRAPHICS_SCISSOR_RECTANGLES,	// Rasterizer (RS) stage
		SET_GRAPHICS_RENDER_TARGET,			// Output-merger (OM) stage
		CLEAR_GRAPHICS,
		DRAW_GRAPHICS,
		DRAW_INDEXED_GRAPHICS,
		DRAW_MESH_TASKS,
		// Compute
		SET_COMPUTE_ROOT_SIGNATURE,
		SET_COMPUTE_PIPELINE_STATE,
		SET_COMPUTE_RESOURCE_GROUP,
		DISPATCH_COMPUTE,
		// Resource
		SET_TEXTURE_MINIMUM_MAXIMUM_MIPMAP_INDEX,
		RESOLVE_MULTISAMPLE_FRAMEBUFFER,
		COPY_RESOURCE,
		GENERATE_MIPMAPS,
		// Query
		RESET_QUERY_POOL,
		BEGIN_QUERY,
		END_QUERY,
		WRITE_TIMESTAMP_QUERY,
		// Debug
		SET_DEBUG_MARKER,
		BEGIN_DEBUG_EVENT,
		END_DEBUG_EVENT,
		// Done
		NUMBER_OF_FUNCTIONS
	};

	typedef void (*ImplementationDispatchFunction)(const void*, IRhi& rhi);
	typedef void* CommandPacket;
	typedef const void* ConstCommandPacket;

	// Global functions
	namespace CommandPacketHelper
	{
		static constexpr uint32_t OFFSET_NEXT_COMMAND_PACKET_BYTE_INDEX = 0u;
		static constexpr uint32_t OFFSET_IMPLEMENTATION_DISPATCH_FUNCTION = OFFSET_NEXT_COMMAND_PACKET_BYTE_INDEX + sizeof(uint32_t);
		static constexpr uint32_t OFFSET_COMMAND = OFFSET_IMPLEMENTATION_DISPATCH_FUNCTION + sizeof(uint32_t);	// Don't use "sizeof(CommandDispatchFunctionIndex)" instead of "sizeof(uint32_t)" so we have a known alignment

		template <typename T>
		[[nodiscard]] inline uint32_t getNumberOfBytes(uint32_t numberOfAuxiliaryBytes)
		{
			return OFFSET_COMMAND + sizeof(T) + numberOfAuxiliaryBytes;
		}

		[[nodiscard]] inline uint32_t getNextCommandPacketByteIndex(const CommandPacket commandPacket)
		{
			return *reinterpret_cast<const uint32_t*>(reinterpret_cast<const uint8_t*>(commandPacket) + OFFSET_NEXT_COMMAND_PACKET_BYTE_INDEX);
		}

		[[nodiscard]] inline uint32_t getNextCommandPacketByteIndex(const ConstCommandPacket constCommandPacket)
		{
			return *reinterpret_cast<const uint32_t*>(reinterpret_cast<const uint8_t*>(constCommandPacket) + OFFSET_NEXT_COMMAND_PACKET_BYTE_INDEX);
		}

		inline void storeNextCommandPacketByteIndex(const CommandPacket commandPacket, uint32_t nextPacketByteIndex)
		{
			*reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(commandPacket) + OFFSET_NEXT_COMMAND_PACKET_BYTE_INDEX) = nextPacketByteIndex;
		}

		[[nodiscard]] inline CommandDispatchFunctionIndex* getCommandDispatchFunctionIndex(const CommandPacket commandPacket)
		{
			return reinterpret_cast<CommandDispatchFunctionIndex*>(reinterpret_cast<uint8_t*>(commandPacket) + OFFSET_IMPLEMENTATION_DISPATCH_FUNCTION);
		}

		[[nodiscard]] inline const CommandDispatchFunctionIndex* getCommandDispatchFunctionIndex(const ConstCommandPacket constCommandPacket)
		{
			return reinterpret_cast<const CommandDispatchFunctionIndex*>(reinterpret_cast<const uint8_t*>(constCommandPacket) + OFFSET_IMPLEMENTATION_DISPATCH_FUNCTION);
		}

		inline void storeImplementationDispatchFunctionIndex(const CommandPacket commandPacket, CommandDispatchFunctionIndex commandDispatchFunctionIndex)
		{
			*getCommandDispatchFunctionIndex(commandPacket) = commandDispatchFunctionIndex;
		}

		[[nodiscard]] inline CommandDispatchFunctionIndex loadCommandDispatchFunctionIndex(const CommandPacket commandPacket)
		{
			return *getCommandDispatchFunctionIndex(commandPacket);
		}

		[[nodiscard]] inline CommandDispatchFunctionIndex loadCommandDispatchFunctionIndex(const ConstCommandPacket constCommandPacket)
		{
			return *getCommandDispatchFunctionIndex(constCommandPacket);
		}

		template <typename T>
		[[nodiscard]] inline T* getCommand(const CommandPacket commandPacket)
		{
			return reinterpret_cast<T*>(reinterpret_cast<uint8_t*>(commandPacket) + OFFSET_COMMAND);
		}

		[[nodiscard]] inline const void* loadCommand(const CommandPacket commandPacket)
		{
			return reinterpret_cast<uint8_t*>(commandPacket) + OFFSET_COMMAND;
		}

		[[nodiscard]] inline const void* loadCommand(const ConstCommandPacket constCommandPacket)
		{
			return reinterpret_cast<const uint8_t*>(constCommandPacket) + OFFSET_COMMAND;
		}

		/**
		*  @brief
		*    Return auxiliary memory address of the given command; returned memory address is considered unstable and might change as soon as another command is added
		*/
		template <typename T>
		[[nodiscard]] inline uint8_t* getAuxiliaryMemory(T* command)
		{
			return reinterpret_cast<uint8_t*>(command) + sizeof(T);
		}

		/**
		*  @brief
		*    Return auxiliary memory address of the given command; returned memory address is considered unstable and might change as soon as another command is added
		*/
		template <typename T>
		[[nodiscard]] inline const uint8_t* getAuxiliaryMemory(const T* command)
		{
			return reinterpret_cast<const uint8_t*>(command) + sizeof(T);
		}
	} // namespace CommandPacketHelper

	/**
	*  @brief
	*    Command buffer
	*
	*  @remarks
	*    Basing on
	*    - "Molecular Musings" - "Stateless, layered, multi-threaded rendering – Part 1" by Stefan Reinalter from November 6, 2014 - https://blog.molecular-matters.com/2014/11/06/stateless-layered-multi-threaded-rendering-part-1/
	*    - "Molecular Musings" - "Stateless, layered, multi-threaded rendering – Part 2" by Stefan Reinalter from November 13, 2014 - http://molecularmusings.wordpress.com/2014/11/13/stateless-layered-multi-threaded-rendering-part-2-stateless-api-design/
	*    - "Molecular Musings" - "Stateless, layered, multi-threaded rendering – Part 3" by Stefan Reinalter from December 16, 2014 - http://molecularmusings.wordpress.com/2014/12/16/stateless-layered-multi-threaded-rendering-part-3-api-design-details/
	*    - "realtimecollisiondetection.net – the blog" - "Order your graphics draw calls around!" by Christer Ericson from October 3, 2008 - http://realtimecollisiondetection.net/blog/?p=86
	*    but without a key inside the more general command buffer. Sorting is a job of a more high level construct like a render queue which also automatically will perform batching and instancing. Also the memory management is much simplified to be cache friendly.
	*
	*  @note
	*    - The commands are stored as a flat contiguous array to be cache friendly
	*    - Each command can have an additional auxiliary buffer, e.g. to store uniform buffer data to submit to the RHI
	*    - It's valid to record a command buffer only once, and submit it multiple times to the RHI
	*/
	class CommandBuffer final
	{
	public:
		inline CommandBuffer() :
			mCommandPacketBufferNumberOfBytes(0),
			mCommandPacketBuffer(nullptr),
			mPreviousCommandPacketByteIndex(~0u),
			mCurrentCommandPacketByteIndex(0)
#if SE_RHI_STATISTICS
			, mNumberOfCommands(0)
#endif
		{
		}

		inline ~CommandBuffer()
		{
			delete[] mCommandPacketBuffer;
		}

		/**
		*  @brief
		*    Return whether or not the command buffer is empty
		*
		*  @return
		*    "true" if the command buffer is empty, else "false"
		*/
		[[nodiscard]] inline bool isEmpty() const
		{
			return (~0u == mPreviousCommandPacketByteIndex);
		}

#if SE_RHI_STATISTICS
		/**
		*  @brief
		*    Return the number of commands inside the command buffer
		*
		*  @return
		*    The number of commands inside the command buffer
		*
		*  @note
		*    - Counting the number of commands inside the command buffer is only a debugging feature and not used in optimized builds
		*/
		[[nodiscard]] inline uint32_t getNumberOfCommands() const
		{
			return mNumberOfCommands;
		}
#endif

		/**
		*  @brief
		*    Return the command packet buffer
		*
		*  @return
		*    The command packet buffer, can be a null pointer, don't destroy the instance
		*
		*  @note
		*    - Internal, don't access the method if you don't have to
		*/
		[[nodiscard]] inline const uint8_t* getCommandPacketBuffer() const
		{
			return (~0u != mPreviousCommandPacketByteIndex) ? mCommandPacketBuffer : nullptr;
		}

		/**
		*  @brief
		*    Clear the command buffer
		*/
		inline void clear()
		{
			mPreviousCommandPacketByteIndex = ~0u;
			mCurrentCommandPacketByteIndex = 0;
#if SE_RHI_STATISTICS
			mNumberOfCommands = 0;
#endif
		}

		/**
		*  @brief
		*    Add command
		*
		*  @param[in] numberOfAuxiliaryBytes
		*    Optional number of auxiliary bytes, e.g. to store uniform buffer data to submit to the RHI
		*
		*  @return
		*    Pointer to the added command, only null pointer in case of apocalypse, don't destroy the memory
		*/
		template <typename U>
		[[nodiscard]] U* addCommand(uint32_t numberOfAuxiliaryBytes = 0)
		{
			// How many command package buffer bytes are consumed by the command to add?
			const uint32_t numberOfCommandBytes = CommandPacketHelper::getNumberOfBytes<U>(numberOfAuxiliaryBytes);

			// 4294967295 is the maximum value of an "uint32_t"-type: Check for overflow
			// -> We use the magic number here to avoid "std::numeric_limits::max()" usage
			RHI_ASSERT((static_cast<uint64_t>(mCurrentCommandPacketByteIndex) + numberOfCommandBytes) < 4294967295u, "Invalid current command packet byte index")

				// Grow command packet buffer, if required
				if ( mCommandPacketBufferNumberOfBytes < mCurrentCommandPacketByteIndex + numberOfCommandBytes )
				{
					// Allocate new memory, grow using a known value but do also add the number of bytes consumed by the current command to add (many auxiliary bytes might be requested)
					const uint32_t newCommandPacketBufferNumberOfBytes = mCommandPacketBufferNumberOfBytes + NUMBER_OF_BYTES_TO_GROW + numberOfCommandBytes;
					uint8_t* newCommandPacketBuffer = new uint8_t[newCommandPacketBufferNumberOfBytes];

					// Copy over current command package buffer content and free it, if required
					if ( nullptr != mCommandPacketBuffer )
					{
						memcpy(newCommandPacketBuffer, mCommandPacketBuffer, mCommandPacketBufferNumberOfBytes);
						delete[] mCommandPacketBuffer;
					}

					// Finalize
					mCommandPacketBuffer = newCommandPacketBuffer;
					mCommandPacketBufferNumberOfBytes = newCommandPacketBufferNumberOfBytes;
				}

			// Get command package for the new command
			CommandPacket commandPacket = &mCommandPacketBuffer[mCurrentCommandPacketByteIndex];

			// Setup previous and current command package
			if ( ~0u != mPreviousCommandPacketByteIndex )
			{
				CommandPacketHelper::storeNextCommandPacketByteIndex(&mCommandPacketBuffer[mPreviousCommandPacketByteIndex], mCurrentCommandPacketByteIndex);
			}
			CommandPacketHelper::storeNextCommandPacketByteIndex(commandPacket, ~0u);
			CommandPacketHelper::storeImplementationDispatchFunctionIndex(commandPacket, U::COMMAND_DISPATCH_FUNCTION_INDEX);
			mPreviousCommandPacketByteIndex = mCurrentCommandPacketByteIndex;
			mCurrentCommandPacketByteIndex += numberOfCommandBytes;

			// Done
#if SE_RHI_STATISTICS
			++mNumberOfCommands;
#endif
			return CommandPacketHelper::getCommand<U>(commandPacket);
		}

		/**
		*  @brief
		*    Submit the command buffer to the RHI without flushing; use this for recording command buffers once and submit them multiple times
		*
		*  @param[in] rhi
		*    RHI to submit the command buffer to
		*/
		inline void submitToRhi(IRhi& rhi) const
		{
			rhi.submitCommandBuffer(*this);
		}

		/**
		*  @brief
		*    Submit the command buffer to the RHI and clear so the command buffer is empty again
		*
		*  @param[in] rhi
		*    RHI to submit the command buffer to
		*/
		inline void submitToRhiAndClear(IRhi& rhi)
		{
			rhi.submitCommandBuffer(*this);
			clear();
		}

		/**
		*  @brief
		*    Submit the command buffer to another command buffer without flushing; use this for recording command buffers once and submit them multiple times
		*
		*  @param[in] commandBuffer
		*    Command buffer to submit the command buffer to
		*/
		inline void submitToCommandBuffer(CommandBuffer& commandBuffer) const
		{
			// Sanity checks
			RHI_ASSERT(this != &commandBuffer, "Can't submit a command buffer to itself");
			RHI_ASSERT(!isEmpty(), "Can't submit empty command buffers");

				// How many command package buffer bytes are consumed by the command to add?
				const uint32_t numberOfCommandBytes = mCurrentCommandPacketByteIndex;

			// 4294967295 is the maximum value of an "uint32_t"-type: Check for overflow
			// -> We use the magic number here to avoid "std::numeric_limits::max()" usage
			RHI_ASSERT((static_cast<uint64_t>(commandBuffer.mCurrentCommandPacketByteIndex) + numberOfCommandBytes) < 4294967295u, "Invalid current command packet byte index");

			// Grow command packet buffer, if required
			if ( commandBuffer.mCommandPacketBufferNumberOfBytes < commandBuffer.mCurrentCommandPacketByteIndex + numberOfCommandBytes )
			{
				// Allocate new memory, grow using a known value but do also add the number of bytes consumed by the current command to add (many auxiliary bytes might be requested)
				const uint32_t newCommandPacketBufferNumberOfBytes = commandBuffer.mCommandPacketBufferNumberOfBytes + NUMBER_OF_BYTES_TO_GROW + numberOfCommandBytes;
				uint8_t* newCommandPacketBuffer = new uint8_t[newCommandPacketBufferNumberOfBytes];

				// Copy over current command package buffer content and free it, if required
				if ( nullptr != commandBuffer.mCommandPacketBuffer )
				{
					memcpy(newCommandPacketBuffer, commandBuffer.mCommandPacketBuffer, commandBuffer.mCommandPacketBufferNumberOfBytes);
					delete[] commandBuffer.mCommandPacketBuffer;
				}

				// Finalize
				commandBuffer.mCommandPacketBuffer = newCommandPacketBuffer;
				commandBuffer.mCommandPacketBufferNumberOfBytes = newCommandPacketBufferNumberOfBytes;
			}

			// Copy over the command buffer in one burst
			memcpy(&commandBuffer.mCommandPacketBuffer[commandBuffer.mCurrentCommandPacketByteIndex], mCommandPacketBuffer, mCurrentCommandPacketByteIndex);

			// Setup previous command package
			if ( ~0u != commandBuffer.mPreviousCommandPacketByteIndex )
			{
				CommandPacketHelper::storeNextCommandPacketByteIndex(&commandBuffer.mCommandPacketBuffer[commandBuffer.mPreviousCommandPacketByteIndex], commandBuffer.mCurrentCommandPacketByteIndex);
			}

			// Update command package indices
			CommandPacket commandPacket = &commandBuffer.mCommandPacketBuffer[commandBuffer.mCurrentCommandPacketByteIndex];
			uint32_t nextCommandPacketByteIndex = CommandPacketHelper::getNextCommandPacketByteIndex(commandPacket);
			while ( ~0u != nextCommandPacketByteIndex )
			{
				nextCommandPacketByteIndex = commandBuffer.mCurrentCommandPacketByteIndex + nextCommandPacketByteIndex;
				CommandPacketHelper::storeNextCommandPacketByteIndex(commandPacket, nextCommandPacketByteIndex);
				commandPacket = &commandBuffer.mCommandPacketBuffer[nextCommandPacketByteIndex];
				nextCommandPacketByteIndex = CommandPacketHelper::getNextCommandPacketByteIndex(commandPacket);
			}

			// Finalize
			commandBuffer.mPreviousCommandPacketByteIndex = commandBuffer.mCurrentCommandPacketByteIndex + mPreviousCommandPacketByteIndex;
			commandBuffer.mCurrentCommandPacketByteIndex += mCurrentCommandPacketByteIndex;
#if SE_RHI_STATISTICS
			commandBuffer.mNumberOfCommands += mNumberOfCommands;
#endif
		}

		/**
		*  @brief
		*    Submit the command buffer to another command buffer and clear so the command buffer is empty again
		*
		*  @param[in] commandBuffer
		*    Command buffer to submit the command buffer to
		*/
		inline void submitToCommandBufferAndClear(CommandBuffer& commandBuffer)
		{
			submitToCommandBuffer(commandBuffer);
			clear();
		}

		// Private definitions
	private:
		static constexpr uint32_t NUMBER_OF_BYTES_TO_GROW = 8192;

		// Private data
	private:
		// Memory
		uint32_t mCommandPacketBufferNumberOfBytes;
		uint8_t* mCommandPacketBuffer;
		// Current state
		uint32_t mPreviousCommandPacketByteIndex;
		uint32_t mCurrentCommandPacketByteIndex;
#if SE_RHI_STATISTICS
		uint32_t mNumberOfCommands;
#endif
	};

	// Concrete commands
	namespace Command
	{

		//[-------------------------------------------------------]
		//[ Command buffer                                        ]
		//[-------------------------------------------------------]
		struct ExecuteCommandBuffer final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, CommandBuffer* commandBufferToExecute)
			{
				RHI_ASSERT(nullptr != commandBufferToExecute, "Invalid command buffer to execute");
				*commandBuffer.addCommand<ExecuteCommandBuffer>() = ExecuteCommandBuffer(commandBufferToExecute);
			}
			// Constructor
			inline explicit ExecuteCommandBuffer(CommandBuffer* _commandBufferToExecute) :
				commandBufferToExecute(_commandBufferToExecute)
			{
			}
			// Data
			CommandBuffer* commandBufferToExecute;
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::EXECUTE_COMMAND_BUFFER;
		};

		//[-------------------------------------------------------]
		//[ Graphics                                              ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Set the used graphics root signature
		*
		*  @param[in] rootSignature
		*    Graphics root signature to use, can be an null pointer (default: "nullptr")
		*/
		struct SetGraphicsRootSignature final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, IRootSignature* rootSignature)
			{
				*commandBuffer.addCommand<SetGraphicsRootSignature>() = SetGraphicsRootSignature(rootSignature);
			}
			// Constructor
			inline explicit SetGraphicsRootSignature(IRootSignature* _rootSignature) :
				rootSignature(_rootSignature)
			{
			}
			// Data
			IRootSignature* rootSignature;
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::SET_GRAPHICS_ROOT_SIGNATURE;
		};

		/**
		*  @brief
		*    Set the used graphics pipeline state
		*
		*  @param[in] graphicsPipelineState
		*    Graphics pipeline state to use, can be an null pointer (default: "nullptr")
		*/
		struct SetGraphicsPipelineState final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, IGraphicsPipelineState* graphicsPipelineState)
			{
				*commandBuffer.addCommand<SetGraphicsPipelineState>() = SetGraphicsPipelineState(graphicsPipelineState);
			}
			// Constructor
			inline explicit SetGraphicsPipelineState(IGraphicsPipelineState* _graphicsPipelineState) :
				graphicsPipelineState(_graphicsPipelineState)
			{
			}
			// Data
			IGraphicsPipelineState* graphicsPipelineState;
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::SET_GRAPHICS_PIPELINE_STATE;
		};

		/**
		*  @brief
		*    Set a graphics resource group
		*
		*  @param[in] rootParameterIndex
		*    The root parameter index number for binding
		*  @param[in] resourceGroup
		*    Resource group to set
		*/
		struct SetGraphicsResourceGroup final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, uint32_t rootParameterIndex, IResourceGroup* resourceGroup)
			{
				*commandBuffer.addCommand<SetGraphicsResourceGroup>() = SetGraphicsResourceGroup(rootParameterIndex, resourceGroup);
			}
			// Constructor
			inline SetGraphicsResourceGroup(uint32_t _rootParameterIndex, IResourceGroup* _resourceGroup) :
				rootParameterIndex(_rootParameterIndex),
				resourceGroup(_resourceGroup)
			{
			}
			// Data
			uint32_t		rootParameterIndex;
			IResourceGroup*	resourceGroup;
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::SET_GRAPHICS_RESOURCE_GROUP;
		};

		/**
		*  @brief
		*    Set the used vertex array, input-assembler (IA) stage
		*
		*  @param[in] vertexArray
		*    Vertex array to use, can be an null pointer (default: "nullptr")
		*/
		struct SetGraphicsVertexArray final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, IVertexArray* vertexArray)
			{
				*commandBuffer.addCommand<SetGraphicsVertexArray>() = SetGraphicsVertexArray(vertexArray);
			}
			// Constructor
			inline explicit SetGraphicsVertexArray(IVertexArray* _vertexArray) :
				vertexArray(_vertexArray)
			{
			}
			// Data
			IVertexArray* vertexArray;
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::SET_GRAPHICS_VERTEX_ARRAY;
		};

		/**
		*  @brief
		*    Set the graphics viewports, rasterizer (RS) stage
		*
		*  @param[in] numberOfViewports
		*    Number of viewports, if <1 nothing happens, must be <="Rhi::Capabilities::maximumNumberOfViewports"
		*  @param[in] viewports
		*    C-array of viewports, there must be at least "numberOfViewports"-viewports, in case of a null pointer nothing happens
		*
		*  @note
		*    - The current viewport(s) does not affect the clear operation
		*    - Lookout! In Direct3D 12 the scissor test can't be deactivated and hence one always needs to set a valid scissor rectangle.
		*      Use the convenience "Rhi::Command::SetGraphicsViewportAndScissorRectangle"-command if possible to not walk into this Direct3D 12 trap.
		*/
		struct SetGraphicsViewports final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, uint32_t numberOfViewports, const Viewport* viewports)
			{
				*commandBuffer.addCommand<SetGraphicsViewports>() = SetGraphicsViewports(numberOfViewports, viewports);
			}
			static inline void create(CommandBuffer& commandBuffer, uint32_t topLeftX, uint32_t topLeftY, uint32_t width, uint32_t height, float minimumDepth = 0.0f, float maximumDepth = 1.0f)
			{
				SetGraphicsViewports* setGraphicsViewportsCommand = commandBuffer.addCommand<SetGraphicsViewports>(sizeof(Viewport));

				// Set command data
				Viewport* viewport = reinterpret_cast<Viewport*>(CommandPacketHelper::getAuxiliaryMemory(setGraphicsViewportsCommand));
				viewport->topLeftX = static_cast<float>(topLeftX);
				viewport->topLeftY = static_cast<float>(topLeftY);
				viewport->width = static_cast<float>(width);
				viewport->height = static_cast<float>(height);
				viewport->minDepth = minimumDepth;
				viewport->maxDepth = maximumDepth;

				// Finalize command
				setGraphicsViewportsCommand->numberOfViewports = 1;
				setGraphicsViewportsCommand->viewports = nullptr;
			}
			// Constructor
			inline SetGraphicsViewports(uint32_t _numberOfViewports, const Viewport* _viewports) :
				numberOfViewports(_numberOfViewports),
				viewports(_viewports)
			{
			}
			// Data
			uint32_t		numberOfViewports;
			const Viewport* viewports;	// If null pointer, command auxiliary memory is used instead
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::SET_GRAPHICS_VIEWPORTS;
		};

		/**
		*  @brief
		*    Set the graphics scissor rectangles, rasterizer (RS) stage
		*
		*  @param[in] numberOfScissorRectangles
		*    Number of scissor rectangles, if <1 nothing happens, must be <="Rhi::Capabilities::maximumNumberOfViewports"
		*  @param[in] dcissorRectangles
		*    C-array of scissor rectangles, there must be at least "numberOfScissorRectangles" scissor rectangles, in case of a null pointer nothing happens
		*
		*  @note
		*    - Scissor rectangles are only used when "Rhi::RasterizerState::scissorEnable" is true
		*    - The current scissor rectangle(s) does not affect the clear operation
		*/
		struct SetGraphicsScissorRectangles final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, uint32_t numberOfScissorRectangles, const ScissorRectangle* scissorRectangles)
			{
				*commandBuffer.addCommand<SetGraphicsScissorRectangles>() = SetGraphicsScissorRectangles(numberOfScissorRectangles, scissorRectangles);
			}
			static inline void create(CommandBuffer& commandBuffer, long topLeftX, long topLeftY, long bottomRightX, long bottomRightY)
			{
				SetGraphicsScissorRectangles* setGraphicsScissorRectanglesCommand = commandBuffer.addCommand<SetGraphicsScissorRectangles>(sizeof(ScissorRectangle));

				// Set command data
				ScissorRectangle* scissorRectangle = reinterpret_cast<ScissorRectangle*>(CommandPacketHelper::getAuxiliaryMemory(setGraphicsScissorRectanglesCommand));
				scissorRectangle->topLeftX = topLeftX;
				scissorRectangle->topLeftY = topLeftY;
				scissorRectangle->bottomRightX = bottomRightX;
				scissorRectangle->bottomRightY = bottomRightY;

				// Finalize command
				setGraphicsScissorRectanglesCommand->numberOfScissorRectangles = 1;
				setGraphicsScissorRectanglesCommand->scissorRectangles = nullptr;
			}
			// Constructor
			inline SetGraphicsScissorRectangles(uint32_t _numberOfScissorRectangles, const ScissorRectangle* _scissorRectangles) :
				numberOfScissorRectangles(_numberOfScissorRectangles),
				scissorRectangles(_scissorRectangles)
			{
			}
			// Data
			uint32_t				numberOfScissorRectangles;
			const ScissorRectangle* scissorRectangles;	// If null pointer, command auxiliary memory is used instead
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::SET_GRAPHICS_SCISSOR_RECTANGLES;
		};

		/**
		*  @brief
		*    Set graphics viewport and scissor rectangle (convenience method)
		*
		*  @param[in] topLeftX
		*    Top left x
		*  @param[in] topLeftY
		*    Top left y
		*  @param[in] width
		*    Width
		*  @param[in] height
		*    Height
		*  @param[in] minimumDepth
		*    Minimum depth
		*  @param[in] maximumDepth
		*    Maximum depth
		*
		*  @note
		*    - Lookout! In Direct3D 12 the scissor test can't be deactivated and hence one always needs to set a valid scissor rectangle.
		*      Use the convenience "Rhi::Command::SetGraphicsViewportAndScissorRectangle"-command if possible to not walk into this Direct3D 12 trap.
		*/
		struct SetGraphicsViewportAndScissorRectangle final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, uint32_t topLeftX, uint32_t topLeftY, uint32_t width, uint32_t height, float minimumDepth = 0.0f, float maximumDepth = 1.0f)
			{
				// Set the graphics viewport
				SetGraphicsViewports::create(commandBuffer, topLeftX, topLeftY, width, height, minimumDepth, maximumDepth);

				// Set the graphics scissor rectangle
				SetGraphicsScissorRectangles::create(commandBuffer, static_cast<long>(topLeftX), static_cast<long>(topLeftY), static_cast<long>(topLeftX + width), static_cast<long>(topLeftY + height));
			}
		};

		/**
		*  @brief
		*    Set the graphics render target to render into, output-merger (OM) stage
		*
		*  @param[in] renderTarget
		*    Render target to render into by binding it to the output-merger state, can be an null pointer to render into the primary window
		*/
		struct SetGraphicsRenderTarget final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, IRenderTarget* renderTarget)
			{
				*commandBuffer.addCommand<SetGraphicsRenderTarget>() = SetGraphicsRenderTarget(renderTarget);
			}
			// Constructor
			inline explicit SetGraphicsRenderTarget(IRenderTarget* _renderTarget) :
				renderTarget(_renderTarget)
			{
			}
			// Data
			IRenderTarget* renderTarget;
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::SET_GRAPHICS_RENDER_TARGET;
		};

		/**
		*  @brief
		*    Clears the graphics viewport to a specified RGBA color, clears the depth buffer,
		*    and erases the stencil buffer
		*
		*  @param[in] clearFlags
		*    Flags that indicate what should be cleared. This parameter can be any
		*    combination of the following flags, but at least one flag must be used:
		*    "Rhi::ClearFlag::COLOR", "Rhi::ClearFlag::DEPTH" and "Rhi::ClearFlag::STENCIL, see "Rhi::ClearFlag"-flags
		*  @param[in] color
		*    RGBA clear color (used if "Rhi::ClearFlag::COLOR" is set)
		*  @param[in] z
		*    Z clear value. (if "Rhi::ClearFlag::DEPTH" is set)
		*    This parameter can be in the range from 0.0 through 1.0. A value of 0.0
		*    represents the nearest distance to the viewer, and 1.0 the farthest distance.
		*  @param[in] stencil
		*    Value to clear the stencil-buffer with. This parameter can be in the range from
		*    0 through 2^n–1, where n is the bit depth of the stencil buffer.
		*
		*  @note
		*    - The current viewport(s) (see "Rhi::Command::SetGraphicsViewports()") does not affect the clear operation
		*    - The current scissor rectangle(s) (see "Rhi::Command::SetGraphicsScissorRectangles()") does not affect the clear operation
		*    - In case there are multiple active render targets, all render targets are cleared
		*/
		struct ClearGraphics final
		{
			// Static methods
			// -> z = 0 instead of 1 due to usage of Reversed-Z (see e.g. https://developer.nvidia.com/content/depth-precision-visualized and https://nlguillemot.wordpress.com/2016/12/07/reversed-z-in-opengl/)
			static inline void create(CommandBuffer& commandBuffer, uint32_t clearFlags, const float color[4], float z = 0.0f, uint32_t stencil = 0)
			{
				*commandBuffer.addCommand<ClearGraphics>() = ClearGraphics(clearFlags, color, z, stencil);
			}
			// Constructor
			inline ClearGraphics(uint32_t _clearFlags, const float _color[4], float _z, uint32_t _stencil) :
				clearFlags(_clearFlags),
				color{ _color[0], _color[1], _color[2], _color[3] },
				z(_z),
				stencil(_stencil)
			{}
			// Data
			uint32_t clearFlags;
			float	 color[4];
			float	 z;
			uint32_t stencil;
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::CLEAR_GRAPHICS;
		};

		/**
		*  @brief
		*    Render the specified geometric primitive, based on an array of vertices instancing and indirect draw
		*
		*  @param[in] indirectBuffer
		*    Indirect buffer to use, the indirect buffer must contain at least "numberOfDraws" instances of "Rhi::DrawArguments" starting at "indirectBufferOffset"
		*  @param[in] indirectBufferOffset
		*    Indirect buffer offset
		*  @param[in] numberOfDraws
		*    Number of draws, can be 0
		*
		*  @note
		*    - Draw instanced is a shader model 4 feature, only supported if "Rhi::Capabilities::drawInstanced" is true
		*    - In Direct3D 9, instanced arrays with hardware support is only possible when drawing indexed primitives, see
		*      "Efficiently Drawing Multiple Instances of Geometry (Direct3D 9)"-article at MSDN: http://msdn.microsoft.com/en-us/library/windows/desktop/bb173349%28v=vs.85%29.aspx#Drawing_Non_Indexed_Geometry
		*    - Fails if no vertex array is set
		*    - If the multi-draw indirect feature is not supported this parameter, multiple draw calls are emitted
		*    - If the draw indirect feature is not supported, a software indirect buffer is used and multiple draw calls are emitted
		*/
		struct DrawGraphics final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, const IIndirectBuffer& indirectBuffer, uint32_t indirectBufferOffset = 0, uint32_t numberOfDraws = 1)
			{
				*commandBuffer.addCommand<DrawGraphics>() = DrawGraphics(indirectBuffer, indirectBufferOffset, numberOfDraws);
			}
			static inline void create(CommandBuffer& commandBuffer, uint32_t vertexCountPerInstance, uint32_t instanceCount = 1, uint32_t startVertexLocation = 0, uint32_t startInstanceLocation = 0)
			{
				DrawGraphics* drawCommand = commandBuffer.addCommand<DrawGraphics>(sizeof(DrawArguments));

				// Set command data: The command packet auxiliary memory contains an "Rhi::DrawArguments"-instance
				const DrawArguments drawArguments(vertexCountPerInstance, instanceCount, startVertexLocation, startInstanceLocation);
				memcpy(CommandPacketHelper::getAuxiliaryMemory(drawCommand), &drawArguments, sizeof(DrawArguments));

				// Finalize command
				drawCommand->indirectBuffer = nullptr;
				drawCommand->indirectBufferOffset = 0;
				drawCommand->numberOfDraws = 1;
			}
			// Constructor
			inline DrawGraphics(const IIndirectBuffer& _indirectBuffer, uint32_t _indirectBufferOffset, uint32_t _numberOfDraws) :
				indirectBuffer(&_indirectBuffer),
				indirectBufferOffset(_indirectBufferOffset),
				numberOfDraws(_numberOfDraws)
			{
			}
			// Data
			const IIndirectBuffer* indirectBuffer;	// If null pointer, command auxiliary memory is used instead
			uint32_t			   indirectBufferOffset;
			uint32_t			   numberOfDraws;
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::DRAW_GRAPHICS;
		};

		/**
		*  @brief
		*    Render the specified geometric primitive, based on indexing into an array of vertices, instancing and indirect draw
		*
		*  @param[in] indirectBuffer
		*    Indirect buffer to use, the indirect buffer must contain at least "numberOfDraws" instances of "Rhi::DrawIndexedArguments" starting at bindirectBufferOffset"
		*  @param[in] indirectBufferOffset
		*    Indirect buffer offset
		*  @param[in] numberOfDraws
		*    Number of draws, can be 0
		*
		*  @note
		*    - Instanced arrays is a shader model 3 feature, only supported if "Rhi::Capabilities::instancedArrays" is true
		*    - Draw instanced is a shader model 4 feature, only supported if "Rhi::Capabilities::drawInstanced" is true
		*    - This method draws indexed primitives from the current set of data input streams
		*    - Fails if no index and/or vertex array is set
		*    - If the multi-draw indirect feature is not supported this parameter, multiple draw calls are emitted
		*    - If the draw indirect feature is not supported, a software indirect buffer is used and multiple draw calls are emitted
		*/
		struct DrawIndexedGraphics final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, const IIndirectBuffer& indirectBuffer, uint32_t indirectBufferOffset = 0, uint32_t numberOfDraws = 1)
			{
				*commandBuffer.addCommand<DrawIndexedGraphics>() = DrawIndexedGraphics(indirectBuffer, indirectBufferOffset, numberOfDraws);
			}
			static inline void create(CommandBuffer& commandBuffer, uint32_t indexCountPerInstance, uint32_t instanceCount = 1, uint32_t startIndexLocation = 0, int32_t baseVertexLocation = 0, uint32_t startInstanceLocation = 0)
			{
				DrawIndexedGraphics* drawCommand = commandBuffer.addCommand<DrawIndexedGraphics>(sizeof(DrawIndexedArguments));

				// Set command data: The command packet auxiliary memory contains an "Rhi::DrawIndexedArguments"-instance
				const DrawIndexedArguments drawIndexedArguments(indexCountPerInstance, instanceCount, startIndexLocation, baseVertexLocation, startInstanceLocation);
				memcpy(CommandPacketHelper::getAuxiliaryMemory(drawCommand), &drawIndexedArguments, sizeof(DrawIndexedArguments));

				// Finalize command
				drawCommand->indirectBuffer = nullptr;
				drawCommand->indirectBufferOffset = 0;
				drawCommand->numberOfDraws = 1;
			}
			// Constructor
			inline DrawIndexedGraphics(const IIndirectBuffer& _indirectBuffer, uint32_t _indirectBufferOffset, uint32_t _numberOfDraws) :
				indirectBuffer(&_indirectBuffer),
				indirectBufferOffset(_indirectBufferOffset),
				numberOfDraws(_numberOfDraws)
			{
			}
			// Data
			const IIndirectBuffer* indirectBuffer;	// If null pointer, command auxiliary memory is used instead
			uint32_t			   indirectBufferOffset;
			uint32_t			   numberOfDraws;
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::DRAW_INDEXED_GRAPHICS;
		};

		/**
		*  @brief
		*    Render the specified geometric primitive, based on a task and mesh shader and indirect draw
		*
		*  @param[in] indirectBuffer
		*    Indirect buffer to use, the indirect buffer must contain at least "numberOfDraws" instances of "Rhi::DrawMeshTasksArguments" starting at "indirectBufferOffset"
		*  @param[in] indirectBufferOffset
		*    Indirect buffer offset
		*  @param[in] numberOfDraws
		*    Number of draws, can be 0
		*
		*  @note
		*    - Only supported if "Rhi::Capabilities::meshShader" is "true"
		*/
		struct DrawMeshTasks final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, const IIndirectBuffer& indirectBuffer, uint32_t indirectBufferOffset = 0, uint32_t numberOfDraws = 1)
			{
				*commandBuffer.addCommand<DrawMeshTasks>() = DrawMeshTasks(indirectBuffer, indirectBufferOffset, numberOfDraws);
			}
			static inline void create(CommandBuffer& commandBuffer, uint32_t numberOfTasks, uint32_t firstTask = 0)
			{
				DrawMeshTasks* drawCommand = commandBuffer.addCommand<DrawMeshTasks>(sizeof(DrawMeshTasksArguments));

				// Set command data: The command packet auxiliary memory contains an "Rhi::DrawMeshTasksArguments"-instance
				const DrawMeshTasksArguments drawArguments(numberOfTasks, firstTask);
				memcpy(CommandPacketHelper::getAuxiliaryMemory(drawCommand), &drawArguments, sizeof(DrawMeshTasksArguments));

				// Finalize command
				drawCommand->indirectBuffer = nullptr;
				drawCommand->indirectBufferOffset = 0;
				drawCommand->numberOfDraws = 1;
			}
			// Constructor
			inline DrawMeshTasks(const IIndirectBuffer& _indirectBuffer, uint32_t _indirectBufferOffset, uint32_t _numberOfDraws) :
				indirectBuffer(&_indirectBuffer),
				indirectBufferOffset(_indirectBufferOffset),
				numberOfDraws(_numberOfDraws)
			{
			}
			// Data
			const IIndirectBuffer* indirectBuffer;	// If null pointer, command auxiliary memory is used instead
			uint32_t			   indirectBufferOffset;
			uint32_t			   numberOfDraws;
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::DRAW_MESH_TASKS;
		};

		//[-------------------------------------------------------]
		//[ Compute                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Set the used compute root signature
		*
		*  @param[in] rootSignature
		*    Compute root signature to use, can be an null pointer (default: "nullptr")
		*
		*  @note
		*    - Only supported if "Rhi::Capabilities::computeShader" is true
		*/
		struct SetComputeRootSignature final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, IRootSignature* rootSignature)
			{
				*commandBuffer.addCommand<SetComputeRootSignature>() = SetComputeRootSignature(rootSignature);
			}
			// Constructor
			inline explicit SetComputeRootSignature(IRootSignature* _rootSignature) :
				rootSignature(_rootSignature)
			{
			}
			// Data
			IRootSignature* rootSignature;
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::SET_COMPUTE_ROOT_SIGNATURE;
		};

		/**
		*  @brief
		*    Set the used compute pipeline state
		*
		*  @param[in] computePipelineState
		*    Compute pipeline state to use, can be an null pointer (default: "nullptr")
		*
		*  @note
		*    - Only supported if "Rhi::Capabilities::computeShader" is true
		*/
		struct SetComputePipelineState final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, IComputePipelineState* computePipelineState)
			{
				*commandBuffer.addCommand<SetComputePipelineState>() = SetComputePipelineState(computePipelineState);
			}
			// Constructor
			inline explicit SetComputePipelineState(IComputePipelineState* _computePipelineState) :
				computePipelineState(_computePipelineState)
			{
			}
			// Data
			IComputePipelineState* computePipelineState;
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::SET_COMPUTE_PIPELINE_STATE;
		};

		/**
		*  @brief
		*    Set a compute resource group
		*
		*  @param[in] rootParameterIndex
		*    The root parameter index number for binding
		*  @param[in] resourceGroup
		*    Resource group to set
		*
		*  @note
		*    - Only supported if "Rhi::Capabilities::computeShader" is true
		*/
		struct SetComputeResourceGroup final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, uint32_t rootParameterIndex, IResourceGroup* resourceGroup)
			{
				*commandBuffer.addCommand<SetComputeResourceGroup>() = SetComputeResourceGroup(rootParameterIndex, resourceGroup);
			}
			// Constructor
			inline SetComputeResourceGroup(uint32_t _rootParameterIndex, IResourceGroup* _resourceGroup) :
				rootParameterIndex(_rootParameterIndex),
				resourceGroup(_resourceGroup)
			{
			}
			// Data
			uint32_t		rootParameterIndex;
			IResourceGroup*	resourceGroup;
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::SET_COMPUTE_RESOURCE_GROUP;
		};

		/**
		*  @brief
		*    Compute dispatch call
		*
		*  @param[in] groupCountX
		*    Number of local workgroups to dispatch in the X dimension
		*  @param[in] groupCountY
		*    Number of local workgroups to dispatch in the Y dimension
		*  @param[in] groupCountZ
		*    Number of local workgroups to dispatch in the Z dimension
		*
		*  @note
		*    - Only supported if "Rhi::Capabilities::computeShader" is true
		*/
		struct DispatchCompute final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ)
			{
				*commandBuffer.addCommand<DispatchCompute>() = DispatchCompute(groupCountX, groupCountY, groupCountZ);
			}
			// Constructor
			inline DispatchCompute(uint32_t _groupCountX, uint32_t _groupCountY, uint32_t _groupCountZ) :
				groupCountX(_groupCountX),
				groupCountY(_groupCountY),
				groupCountZ(_groupCountZ)
			{
			}
			// Data
			uint32_t groupCountX;
			uint32_t groupCountY;
			uint32_t groupCountZ;
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::DISPATCH_COMPUTE;
		};

		//[-------------------------------------------------------]
		//[ Resource                                              ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Set texture minimum maximum mipmap index
		*
		*  @param[in] texture
		*    Texture to set the minimum maximum mipmap index of
		*  @param[in] minimumMipmapIndex
		*    Minimum mipmap index, the most detailed mipmap, also known as base mipmap, 0 by default
		*  @param[in] maximumMipmapIndex
		*    Maximum mipmap index, the least detailed mipmap, <number of mipmaps> by default
		*/
		struct SetTextureMinimumMaximumMipmapIndex final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, ITexture& texture, uint32_t minimumMipmapIndex, uint32_t maximumMipmapIndex)
			{
				*commandBuffer.addCommand<SetTextureMinimumMaximumMipmapIndex>() = SetTextureMinimumMaximumMipmapIndex(texture, minimumMipmapIndex, maximumMipmapIndex);
			}
			// Constructor
			inline SetTextureMinimumMaximumMipmapIndex(ITexture& _texture, uint32_t _minimumMipmapIndex, uint32_t _maximumMipmapIndex) :
				texture(&_texture),
				minimumMipmapIndex(_minimumMipmapIndex),
				maximumMipmapIndex(_maximumMipmapIndex)
			{
			}
			// Data
			ITexture* texture;
			uint32_t  minimumMipmapIndex;
			uint32_t  maximumMipmapIndex;
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::SET_TEXTURE_MINIMUM_MAXIMUM_MIPMAP_INDEX;
		};

		/**
		*  @brief
		*    Resolve multisample framebuffer
		*
		*  @param[in] destinationRenderTarget
		*    None multisample destination render target
		*  @param[in] sourceMultisampleFramebuffer
		*    Source multisample framebuffer
		*/
		struct ResolveMultisampleFramebuffer final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, IRenderTarget& destinationRenderTarget, IFramebuffer& sourceMultisampleFramebuffer)
			{
				*commandBuffer.addCommand<ResolveMultisampleFramebuffer>() = ResolveMultisampleFramebuffer(destinationRenderTarget, sourceMultisampleFramebuffer);
			}
			// Constructor
			inline ResolveMultisampleFramebuffer(IRenderTarget& _destinationRenderTarget, IFramebuffer& _sourceMultisampleFramebuffer) :
				destinationRenderTarget(&_destinationRenderTarget),
				sourceMultisampleFramebuffer(&_sourceMultisampleFramebuffer)
			{
			}
			// Data
			IRenderTarget* destinationRenderTarget;
			IFramebuffer* sourceMultisampleFramebuffer;
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::RESOLVE_MULTISAMPLE_FRAMEBUFFER;
		};

		/**
		*  @brief
		*    Copy resource
		*
		*  @param[in] destinationResource
		*    Destination resource
		*  @param[in] sourceResource
		*    Source Resource
		*/
		struct CopyResource final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, IResource& destinationResource, IResource& sourceResource)
			{
				*commandBuffer.addCommand<CopyResource>() = CopyResource(destinationResource, sourceResource);
			}
			// Constructor
			inline CopyResource(IResource& _destinationResource, IResource& _sourceResource) :
				destinationResource(&_destinationResource),
				sourceResource(&_sourceResource)
			{
			}
			// Data
			IResource* destinationResource;
			IResource* sourceResource;
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::COPY_RESOURCE;
		};

		/**
		*  @brief
		*    Generate resource mipmaps
		*
		*  @param[in] resource
		*    Resource
		*/
		struct GenerateMipmaps final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, IResource& resource)
			{
				*commandBuffer.addCommand<GenerateMipmaps>() = GenerateMipmaps(resource);
			}
			// Constructor
			inline explicit GenerateMipmaps(IResource& _resource) :
				resource(&_resource)
			{
			}
			// Data
			IResource* resource;
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::GENERATE_MIPMAPS;
		};

		//[-------------------------------------------------------]
		//[ Query                                                 ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Reset asynchronous query pool
		*
		*  @param[in] queryPool
		*    Query pool
		*  @param[in] firstQueryIndex
		*    First query index (e.g. 0)
		*  @param[in] numberOfQueries
		*    Number of queries (e.g. 1)
		*/
		struct ResetQueryPool final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, IQueryPool& queryPool, uint32_t firstQueryIndex = 0, uint32_t numberOfQueries = 1)
			{
				*commandBuffer.addCommand<ResetQueryPool>() = ResetQueryPool(queryPool, firstQueryIndex, numberOfQueries);
			}
			// Constructor
			inline ResetQueryPool(IQueryPool& _queryPool, uint32_t _firstQueryIndex, uint32_t _numberOfQueries) :
				queryPool(&_queryPool),
				firstQueryIndex(_firstQueryIndex),
				numberOfQueries(_numberOfQueries)
			{
			}
			// Data
			IQueryPool* queryPool;
			uint32_t	firstQueryIndex;
			uint32_t	numberOfQueries;
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::RESET_QUERY_POOL;
		};

		/**
		*  @brief
		*    Begin asynchronous query
		*
		*  @param[in] queryPool
		*    Query pool
		*  @param[in] queryIndex
		*    Query index (e.g. 0)
		*  @param[in] queryControlFlags
		*    Query control flags (e.g. "Rhi::QueryControlFlags::PRECISE")
		*/
		struct BeginQuery final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, IQueryPool& queryPool, uint32_t queryIndex = 0, uint32_t queryControlFlags = 0)
			{
				*commandBuffer.addCommand<BeginQuery>() = BeginQuery(queryPool, queryIndex, queryControlFlags);
			}
			// Constructor
			inline BeginQuery(IQueryPool& _queryPool, uint32_t _queryIndex, uint32_t _queryControlFlags) :
				queryPool(&_queryPool),
				queryIndex(_queryIndex),
				queryControlFlags(_queryControlFlags)
			{
			}
			// Data
			IQueryPool* queryPool;
			uint32_t	queryIndex;
			uint32_t	queryControlFlags;
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::BEGIN_QUERY;
		};

		/**
		*  @brief
		*    Reset and begin asynchronous query; ease-of-use function in case there's just a single query inside the query pool
		*
		*  @param[in] queryPool
		*    Query pool
		*  @param[in] queryIndex
		*    Query index (e.g. 0)
		*  @param[in] queryControlFlags
		*    Query control flags (e.g. "Rhi::QueryControlFlags::PRECISE")
		*/
		struct ResetAndBeginQuery final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, IQueryPool& queryPool, uint32_t queryIndex = 0, uint32_t queryControlFlags = 0)
			{
				*commandBuffer.addCommand<ResetQueryPool>() = ResetQueryPool(queryPool, queryIndex, 1);
				*commandBuffer.addCommand<BeginQuery>() = BeginQuery(queryPool, queryIndex, queryControlFlags);
			}
		};

		/**
		*  @brief
		*    End asynchronous query
		*
		*  @param[in] queryPool
		*    Query pool
		*  @param[in] queryIndex
		*    Query index (e.g. 0)
		*/
		struct EndQuery final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, IQueryPool& queryPool, uint32_t queryIndex = 0)
			{
				*commandBuffer.addCommand<EndQuery>() = EndQuery(queryPool, queryIndex);
			}
			// Constructor
			inline EndQuery(IQueryPool& _queryPool, uint32_t _queryIndex) :
				queryPool(&_queryPool),
				queryIndex(_queryIndex)
			{
			}
			// Data
			IQueryPool* queryPool;
			uint32_t	queryIndex;
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::END_QUERY;
		};

		/**
		*  @brief
		*    Write asynchronous timestamp query
		*
		*  @param[in] queryPool
		*    Query pool
		*  @param[in] queryIndex
		*    Query index (e.g. 0)
		*/
		struct WriteTimestampQuery final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, IQueryPool& queryPool, uint32_t queryIndex = 0)
			{
				*commandBuffer.addCommand<WriteTimestampQuery>() = WriteTimestampQuery(queryPool, queryIndex);
			}
			// Constructor
			inline WriteTimestampQuery(IQueryPool& _queryPool, uint32_t _queryIndex) :
				queryPool(&_queryPool),
				queryIndex(_queryIndex)
			{
			}
			// Data
			IQueryPool* queryPool;
			uint32_t	queryIndex;
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::WRITE_TIMESTAMP_QUERY;
		};

		//[-------------------------------------------------------]
		//[ Debug                                                 ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Set a debug marker
		*
		*  @param[in] name
		*    ASCII name of the debug marker, must be valid (there's no internal null pointer test)
		*
		*  @see
		*    - "isDebugEnabled()"
		*/
		struct SetDebugMarker final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, const char* name)
			{
				*commandBuffer.addCommand<SetDebugMarker>() = SetDebugMarker(name);
			}
			// Constructor
			inline explicit SetDebugMarker(const char* _name)
			{
				RHI_ASSERT(strlen(_name) < 128, "Invalid name")
					strncpy(name, _name, 128);
				name[127] = '\0';
			}
			// Data
			char name[128];
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::SET_DEBUG_MARKER;
		};

		/**
		*  @brief
		*    Begin debug event
		*
		*  @param[in] name
		*    ASCII name of the debug event, must be valid (there's no internal null pointer test)
		*
		*  @see
		*    - "isDebugEnabled()"
		*/
		struct BeginDebugEvent final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, const char* name)
			{
				*commandBuffer.addCommand<BeginDebugEvent>() = BeginDebugEvent(name);
			}
			// Constructor
			inline explicit BeginDebugEvent(const char* _name)
			{
				RHI_ASSERT(strlen(_name) < 128, "Invalid name")
					strncpy(name, _name, 128);
				name[127] = '\0';
			}
			// Data
			char name[128];
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::BEGIN_DEBUG_EVENT;
		};

		/**
		*  @brief
		*    End the last started debug event
		*
		*  @see
		*    - "isDebugEnabled()"
		*/
		struct EndDebugEvent final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer)
			{
				*commandBuffer.addCommand<EndDebugEvent>() = EndDebugEvent();
			}
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::END_DEBUG_EVENT;
		};

	}

	// Debug macros
#if SE_DEBUG
	/**
	*  @brief
	*    Set a debug marker
	*
	*  @param[in] commandBuffer
	*    Reference to the RHI instance to use
	*  @param[in] name
	*    ASCII name of the debug marker
	*/
#define COMMAND_SET_DEBUG_MARKER(commandBuffer, name) Rhi::Command::SetDebugMarker::create(commandBuffer, name);

	/**
	*  @brief
	*    Set a debug marker by using the current function name ("__FUNCTION__") as marker name
	*
	*  @param[in] commandBuffer
	*    Reference to the RHI instance to use
	*/
#define COMMAND_SET_DEBUG_MARKER_FUNCTION(commandBuffer) Rhi::Command::SetDebugMarker::create(commandBuffer, __FUNCTION__);

	/**
	*  @brief
	*    Begin debug event
	*
	*  @param[in] commandBuffer
	*    Reference to the RHI instance to use, must be ended by using "COMMAND_END_DEBUG_EVENT()"
	*  @param[in] name
	*    ASCII name of the debug event
	*/
#define COMMAND_BEGIN_DEBUG_EVENT(commandBuffer, name) Rhi::Command::BeginDebugEvent::create(commandBuffer, name);

	/**
	*  @brief
	*    Begin debug event by using the current function name ("__FUNCTION__") as event name, must be ended by using "COMMAND_END_DEBUG_EVENT()"
	*
	*  @param[in] commandBuffer
	*    Reference to the RHI instance to use
	*/
#define COMMAND_BEGIN_DEBUG_EVENT_FUNCTION(commandBuffer) Rhi::Command::BeginDebugEvent::create(commandBuffer, __FUNCTION__);

	/**
	*  @brief
	*    End the last started debug event
	*
	*  @param[in] commandBuffer
	*    Reference to the RHI instance to use
	*/
#define COMMAND_END_DEBUG_EVENT(commandBuffer) Rhi::Command::EndDebugEvent::create(commandBuffer);

	/**
	*  @brief
	*    Scoped debug event class
	*/
	class CommandScopedDebugEventOnExit
	{
		// Public methods
	public:
		inline explicit CommandScopedDebugEventOnExit(CommandBuffer& commandBuffer) :
			mCommandBuffer(commandBuffer)
		{
		}

		inline ~CommandScopedDebugEventOnExit()
		{
			Rhi::Command::EndDebugEvent::create(mCommandBuffer);
		}

		// Private methods
	private:
		explicit CommandScopedDebugEventOnExit(const CommandScopedDebugEventOnExit& commandScopedDebugEventOnExit) = delete;
		CommandScopedDebugEventOnExit& operator =(const CommandScopedDebugEventOnExit& commandScopedDebugEventOnExit) = delete;

		// Private data
	private:
		CommandBuffer& mCommandBuffer;
	};

	/**
	*  @brief
	*    Scoped debug event, minor internal overhead compared to manual begin/end
	*
	*  @param[in] commandBuffer
	*    Reference to the RHI instance to use
	*  @param[in] name
	*    ASCII name of the debug event
	*/
#define COMMAND_SCOPED_DEBUG_EVENT(commandBuffer, name) \
			Rhi::Command::BeginDebugEvent::create(commandBuffer, name); \
			SE_PRAGMA_WARNING_PUSH \
				SE_PRAGMA_WARNING_DISABLE_MSVC(4456) \
				Rhi::CommandScopedDebugEventOnExit commandScopedDebugEventOnExit##__FUNCTION__(commandBuffer); \
			SE_PRAGMA_WARNING_POP

	/**
	*  @brief
	*    Scoped debug event by using the current function name ("__FUNCTION__") as event name, minor internal overhead compared to manual begin/end
	*
	*  @param[in] commandBuffer
	*    Reference to the RHI instance to use
	*/
#define COMMAND_SCOPED_DEBUG_EVENT_FUNCTION(commandBuffer) \
			Rhi::Command::BeginDebugEvent::create(commandBuffer, __FUNCTION__); \
			SE_PRAGMA_WARNING_PUSH \
				SE_PRAGMA_WARNING_DISABLE_MSVC(4456) \
				Rhi::CommandScopedDebugEventOnExit commandScopedDebugEventOnExit##__FUNCTION__(commandBuffer); \
			SE_PRAGMA_WARNING_POP
#else
	/**
	*  @brief
	*    Set a debug marker
	*
	*  @param[in] commandBuffer
	*    Reference to the RHI instance to use
	*  @param[in] name
	*    ASCII name of the debug marker
	*/
#define COMMAND_SET_DEBUG_MARKER(commandBuffer, name)

	/**
	*  @brief
	*    Set a debug marker by using the current function name ("__FUNCTION__") as marker name
	*
	*  @param[in] commandBuffer
	*    Reference to the RHI instance to use
	*/
#define COMMAND_SET_DEBUG_MARKER_FUNCTION(commandBuffer)

	/**
	*  @brief
	*    Begin debug event, must be ended by using "COMMAND_END_DEBUG_EVENT()"
	*
	*  @param[in] commandBuffer
	*    Reference to the RHI instance to use
	*  @param[in] name
	*    ASCII name of the debug event
	*/
#define COMMAND_BEGIN_DEBUG_EVENT(commandBuffer, name)

	/**
	*  @brief
	*    Begin debug event by using the current function name ("__FUNCTION__") as event name, must be ended by using "COMMAND_END_DEBUG_EVENT()"
	*
	*  @param[in] commandBuffer
	*    Reference to the RHI instance to use
	*/
#define COMMAND_BEGIN_DEBUG_EVENT_FUNCTION(commandBuffer)

	/**
	*  @brief
	*    End the last started debug event
	*
	*  @param[in] commandBuffer
	*    Reference to the RHI instance to use
	*/
#define COMMAND_END_DEBUG_EVENT(commandBuffer)

	/**
	*  @brief
	*    Scoped debug event, minor internal overhead compared to manual begin/end
	*
	*  @param[in] commandBuffer
	*    Reference to the RHI instance to use
	*  @param[in] name
	*    ASCII name of the debug event
	*/
#define COMMAND_SCOPED_DEBUG_EVENT(commandBuffer, name)

	/**
	*  @brief
	*    Scoped debug event by using the current function name ("__FUNCTION__") as event name, minor internal overhead compared to manual begin/end
	*
	*  @param[in] commandBuffer
	*    Reference to the RHI instance to use
	*/
#define COMMAND_SCOPED_DEBUG_EVENT_FUNCTION(commandBuffer)
#endif
} // namespace Rhi