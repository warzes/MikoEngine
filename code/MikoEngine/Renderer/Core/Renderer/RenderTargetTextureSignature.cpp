#include "stdafx.h"
#include "Renderer/Core/Renderer/RenderTargetTextureSignature.h"
#include "Renderer/Core/Math/Math.h"
#include "RHI/TextureTypes.h"

namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	RenderTargetTextureSignature::RenderTargetTextureSignature(uint32_t width, uint32_t height, Rhi::TextureFormat::Enum textureFormat, uint8_t flags, float widthScale, float heightScale) :
		mWidth(width),
		mHeight(height),
		mTextureFormat(textureFormat),
		mFlags(flags),
		mWidthScale(widthScale),
		mHeightScale(heightScale),
		mRenderTargetTextureSignatureId(Math::FNV1a_INITIAL_HASH_32)
	{
		mRenderTargetTextureSignatureId = Math::calculateFNV1a32(reinterpret_cast<const uint8_t*>(&mWidth), sizeof(uint32_t), mRenderTargetTextureSignatureId);
		mRenderTargetTextureSignatureId = Math::calculateFNV1a32(reinterpret_cast<const uint8_t*>(&mHeight), sizeof(uint32_t), mRenderTargetTextureSignatureId);
		mRenderTargetTextureSignatureId = Math::calculateFNV1a32(reinterpret_cast<const uint8_t*>(&mTextureFormat), sizeof(Rhi::TextureFormat::Enum), mRenderTargetTextureSignatureId);
		mRenderTargetTextureSignatureId = Math::calculateFNV1a32(reinterpret_cast<const uint8_t*>(&mFlags), sizeof(uint8_t), mRenderTargetTextureSignatureId);
		mRenderTargetTextureSignatureId = Math::calculateFNV1a32(reinterpret_cast<const uint8_t*>(&mWidthScale), sizeof(float), mRenderTargetTextureSignatureId);
		mRenderTargetTextureSignatureId = Math::calculateFNV1a32(reinterpret_cast<const uint8_t*>(&mHeightScale), sizeof(float), mRenderTargetTextureSignatureId);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer
