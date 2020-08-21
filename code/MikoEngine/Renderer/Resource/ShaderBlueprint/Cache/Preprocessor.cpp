#include "stdafx.h"
#include "Renderer/Resource/ShaderBlueprint/Cache/Preprocessor.h"
#define MOJOSHADER_NO_VERSION_INCLUDE
#include "Renderer/IRenderer.h"
#include "Renderer/Context.h"

#include <MojoShader/mojoshader.h>

#include <cstring>	// For "strlen()"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	void Preprocessor::preprocess(const IRenderer& renderer, std::string& source, std::string& result)
	{
		// TODO(co) The usage of MojoShader just as preprocessor is overkill. Find a simpler but still efficient solution. Switch to "mcpp -- a portable C preprocessor" ( http://mcpp.sourceforge.net/ ) ?

		// Disable warnings in external headers, we can't fix them
		SE_PRAGMA_WARNING_PUSH
			SE_PRAGMA_WARNING_DISABLE_MSVC(5039)	// warning C5039: 'MOJOSHADER_preprocess': pointer or reference to potentially throwing function passed to extern C function under -EHc. Undefined behavior may occur if this function throws an exception.

			// Preprocess
			const MOJOSHADER_preprocessData* preprocessData = MOJOSHADER_preprocess(nullptr, source.c_str(), static_cast<unsigned int>(source.length()), 0, 0, 0, 0, 0, 0, 0, 0, 0);
		SE_PRAGMA_WARNING_POP

		// Evaluate the result
		if (preprocessData->error_count > 0)
		{
			for (int i = 0; i < preprocessData->error_count; ++i)
			{
				RHI_LOG(CRITICAL, "Renderer shader preprocessor %s:%d: Error: %s", preprocessData->errors[i].filename ? preprocessData->errors[i].filename : "???", preprocessData->errors[i].error_position, preprocessData->errors[i].error)
			}
		}
		else
		{
			result.assign(preprocessData->output, static_cast<size_t>(preprocessData->output_len));
		}
		MOJOSHADER_freePreprocessData(preprocessData);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer
