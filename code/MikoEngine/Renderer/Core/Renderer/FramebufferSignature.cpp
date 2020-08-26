#include "stdafx.h"
#include "Renderer/Core/Renderer/FramebufferSignature.h"
#include "Math/Math.h"

namespace Renderer
{
	FramebufferSignature::FramebufferSignature(uint8_t numberOfColorAttachments, const FramebufferSignatureAttachment colorFramebufferSignatureAttachments[8], const FramebufferSignatureAttachment& depthStencilFramebufferSignatureAttachment) :
		mNumberOfColorAttachments(numberOfColorAttachments),
		mColorFramebufferSignatureAttachments{ colorFramebufferSignatureAttachments[0], colorFramebufferSignatureAttachments[1], colorFramebufferSignatureAttachments[2], colorFramebufferSignatureAttachments[3], colorFramebufferSignatureAttachments[4], colorFramebufferSignatureAttachments[5], colorFramebufferSignatureAttachments[6], colorFramebufferSignatureAttachments[7] },
		mDepthStencilFramebufferSignatureAttachment(depthStencilFramebufferSignatureAttachment),
		mFramebufferSignatureId(Math::FNV1a_INITIAL_HASH_32)
	{
		mFramebufferSignatureId = Math::calculateFNV1a32(reinterpret_cast<const uint8_t*>(&mNumberOfColorAttachments), sizeof(uint32_t), mFramebufferSignatureId);
		for ( uint8_t i = 0; i < mNumberOfColorAttachments; ++i )
		{
			mFramebufferSignatureId = Math::calculateFNV1a32(reinterpret_cast<const uint8_t*>(&mColorFramebufferSignatureAttachments[i]), sizeof(FramebufferSignatureAttachment), mFramebufferSignatureId);
		}
		mFramebufferSignatureId = Math::calculateFNV1a32(reinterpret_cast<const uint8_t*>(&mDepthStencilFramebufferSignatureAttachment), sizeof(FramebufferSignatureAttachment), mFramebufferSignatureId);
	}
} // Renderer