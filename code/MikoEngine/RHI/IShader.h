#pragma once

namespace Rhi
{
	class IShader : public IResource
	{

		// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~IShader() override
		{
		}

		// Public virtual Rhi::IShader methods
	public:
		/**
		*  @brief
		*    Return the name of the shader language the shader is using
		*
		*  @return
		*    The ASCII name of the shader language the shader is using (for example "GLSL" or "HLSL"), always valid
		*
		*  @note
		*    - Do not free the memory the returned pointer is pointing to
		*/
		[[nodiscard]] virtual const char* getShaderLanguageName() const = 0;

		// Protected methods
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] resourceType
		*    Resource type
		*  @param[in] rhi
		*    Owner RHI instance
		*/
		inline IShader(ResourceType resourceType, IRhi& rhi RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IResource(resourceType, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
		}

		explicit IShader(const IShader& source) = delete;
		IShader& operator =(const IShader& source) = delete;

	};

	typedef SmartRefCount<IShader> IShaderPtr;
} // namespace Rhi