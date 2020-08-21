#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]


// Disable warnings in external headers, we can't fix them
SE_PRAGMA_WARNING_PUSH
	SE_PRAGMA_WARNING_DISABLE_MSVC(4127)	// warning C4127: conditional expression is constant
	SE_PRAGMA_WARNING_DISABLE_MSVC(4201)	// warning C4201: nonstandard extension used: nameless struct/union
	SE_PRAGMA_WARNING_DISABLE_MSVC(4464)	// warning C4464: relative include path contains '..'
	SE_PRAGMA_WARNING_DISABLE_MSVC(4324)	// warning C4324: '<x>': structure was padded due to alignment specifier
	#include <glm/glm.hpp>
SE_PRAGMA_WARNING_POP


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
	*    Hosek-Wilkie sky
	*
	*  @note
	*    - "An Analytic Model for Full Spectral Sky-Dome Radiance (Lukas Hosek, Alexander Wilkie)"
	*    - The implementation is basing on https://github.com/ddiakopoulos/sandbox/blob/master/assets/shaders/sky_hosek_frag.glsl (has also a Preetham implementation)
	*/
	class HosekWilkieSky final
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		struct Coefficients final
		{
			glm::vec3 A, B, C, D, E, F, G, H, I;
			glm::vec3 Z;
			float	  padding[2];	// Padding so this data structure is 32 bytes (2 * float4x4)
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		HosekWilkieSky();

		inline ~HosekWilkieSky()
		{
			// Nothing here
		}

		[[nodiscard]] inline const Coefficients& getCoefficients() const
		{
			return mCoefficients;
		}

		[[nodiscard]] inline const glm::vec3& getSunColor() const
		{
			return mSunColor;
		}

		void recalculate(const glm::vec3& worldSpaceSunDirection, float turbidity = 4.0f, float albedo = 0.1f, float normalizedSunY = 1.15f);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		glm::vec3	 mWorldSpaceSunDirection;
		float		 mTurbidity;
		float		 mAlbedo;
		float		 mNormalizedSunY;
		Coefficients mCoefficients;
		glm::vec3	 mSunColor;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer
