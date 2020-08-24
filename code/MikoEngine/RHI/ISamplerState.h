#pragma once

namespace Rhi
{
	class ISamplerState : public IState
	{

		// Public static methods
	public:
		/**
		*  @brief
		*    Return the default sampler state
		*
		*  @return
		*    The default sampler state, see "Rhi::SamplerState" for the default values
		*/
		[[nodiscard]] static inline const SamplerState& getDefaultSamplerState()
		{
			// As default values, the one of Direct3D 11 and Direct 10 were chosen in order to make it easier for those RHI implementations
			// (choosing OpenGL default values would bring no benefit due to the design of the OpenGL API)

			// Direct3D 11 "D3D11_SAMPLER_DESC structure"-documentation at MSDN: http://msdn.microsoft.com/en-us/library/windows/desktop/ff476207%28v=vs.85%29.aspx

			// The Direct3D 10 documentation is buggy: (online and within the "Microsoft DirectX SDK (June 2010)"-SDK, checked it)
			//   - "D3D10_SAMPLER_DESC structure"-documentation at MSDN: http://msdn.microsoft.com/en-us/library/windows/desktop/bb172415%28v=vs.85%29.aspx
			//     -> Says "Default filter is Min_Mag_Mip_Point"
			//   - "ID3D10Device::VSSetSamplers method"-documentation at MSDN: msdn.microsoft.com/en-us/library/windows/desktop/bb173627(v=vs.85).aspx
			//     -> Says "Default filter is Min_Mag_Mip_Linear"
			//   -> When testing the behaviour, it "looks like" Min_Mag_Mip_Linear is used

			// Direct3D 9 "D3DSAMPLERSTATETYPE enumeration"-documentation at MSDN: http://msdn.microsoft.com/en-us/library/windows/desktop/bb172602%28v=vs.85%29.aspx

			// OpenGL & OpenGL ES 3: The official specifications (unlike Direct3D, OpenGL versions are more compatible to each other)

			// Return default values
			static constexpr SamplerState SAMPLER_STATE =
			{																				//	Direct3D 11					Direct3D 10						Direct3D 9				OpenGL
				FilterMode::MIN_MAG_MIP_LINEAR,	// filter (Rhi::FilterMode)					"MIN_MAG_MIP_LINEAR"			"MIN_MAG_MIP_LINEAR"			"MIN_MAG_MIP_POINT"		"MIN_POINT_MAG_MIP_LINEAR"
				TextureAddressMode::CLAMP,		// addressU (Rhi::TextureAddressMode)		"CLAMP"							"CLAMP"							"WRAP"					"WRAP"
				TextureAddressMode::CLAMP,		// addressV (Rhi::TextureAddressMode)		"CLAMP"							"CLAMP"							"WRAP"					"WRAP"
				TextureAddressMode::CLAMP,		// addressW (Rhi::TextureAddressMode)		"CLAMP"							"CLAMP"							"WRAP"					"WRAP"
				0.0f,							// mipLodBias (float)						"0.0f"							"0.0f"							"0.0f"					"0.0f"
				16,								// maxAnisotropy (uint32_t)					"16"							"16"							"1"						"1"
				ComparisonFunc::NEVER,			// comparisonFunc (Rhi::ComparisonFunc)		"NEVER"							"NEVER"							<unsupported>			"LESS_EQUAL"
				{
					0.0f,						// borderColor[0] (float)					"0.0f"							"0.0f"							"0.0f"					"0.0f"
					0.0f,						// borderColor[1] (float)					"0.0f"							"0.0f"							"0.0f"					"0.0f"
					0.0f,						// borderColor[2] (float)					"0.0f"							"0.0f"							"0.0f"					"0.0f"
					0.0f						// borderColor[3] (float)					"0.0f"							"0.0f"							"0.0f"					"0.0f"
				},
				-3.402823466e+38f,				// minLod (float) - Default: -FLT_MAX		"-3.402823466e+38F (-FLT_MAX)"	"-3.402823466e+38F (-FLT_MAX)"	<unsupported>			"-1000.0f"
				3.402823466e+38f				// maxLod (float) - Default: FLT_MAX		"3.402823466e+38F (FLT_MAX)"	"3.402823466e+38F (FLT_MAX)"	"0.0f"					"1000.0f"
			};
			return SAMPLER_STATE;
		}

		// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~ISamplerState() override
		{
#ifdef SE_RHI_STATISTICS
			// Update the statistics
			--getRhi().getStatistics().currentNumberOfSamplerStates;
#endif
		}

		// Protected methods
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] rhi
		*    Owner RHI instance
		*/
		inline explicit ISamplerState(IRhi& rhi RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IState(ResourceType::SAMPLER_STATE, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
#ifdef SE_RHI_STATISTICS
			// Update the statistics
			++rhi.getStatistics().numberOfCreatedSamplerStates;
			++rhi.getStatistics().currentNumberOfSamplerStates;
#endif
		}

		explicit ISamplerState(const ISamplerState& source) = delete;
		ISamplerState& operator =(const ISamplerState& source) = delete;

	};

	typedef SmartRefCount<ISamplerState> ISamplerStatePtr;
} // namespace Rhi