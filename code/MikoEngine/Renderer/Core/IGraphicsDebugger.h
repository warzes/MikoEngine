#pragma once

#include "RHI/Rhi.h"


#ifdef RENDERER_GRAPHICS_DEBUGGER


	//[-------------------------------------------------------]
	//[ Includes                                              ]
	//[-------------------------------------------------------]
	#include "Rhi/Rhi.h"


	//[-------------------------------------------------------]
	//[ Namespace                                             ]
	//[-------------------------------------------------------]
	namespace Renderer
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Abstract graphics debugger interface
		*/
		class IGraphicsDebugger
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			[[nodiscard]] inline bool getCaptureNextFrame() const
			{
				return mCaptureNextFrame;
			}

			inline void captureNextFrame()
			{
				mCaptureNextFrame = true;
			}


		//[-------------------------------------------------------]
		//[ Public virtual Renderer::IGraphicsDebugger methods    ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Return whether or not the graphics debugger instance is properly initialized
			*
			*  @return
			*    "true" if the graphics debugger instance is properly initialized, else "false"
			*/
			[[nodiscard]] virtual bool isInitialized() const = 0;

			/**
			*  @brief
			*    Start frame capture
			*
			*  @param[in] nativeWindowHandle
			*    Native RHI window handle
			*/
			virtual void startFrameCapture(handle nativeWindowHandle) = 0;

			/**
			*  @brief
			*    End frame capture
			*
			*  @param[in] nativeWindowHandle
			*    Native RHI window handle
			*/
			virtual void endFrameCapture(handle nativeWindowHandle) = 0;


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
		protected:
			inline IGraphicsDebugger() :
				mCaptureNextFrame(false)
			{
				// Nothing here
			}

			inline virtual ~IGraphicsDebugger()
			{
				// Nothing here
			}

			explicit IGraphicsDebugger(const IGraphicsDebugger&) = delete;
			IGraphicsDebugger& operator=(const IGraphicsDebugger&) = delete;

			inline void resetCaptureNextFrame()
			{
				mCaptureNextFrame = false;
			}


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			bool mCaptureNextFrame;


		};


	//[-------------------------------------------------------]
	//[ Namespace                                             ]
	//[-------------------------------------------------------]
	} // Renderer


#endif
