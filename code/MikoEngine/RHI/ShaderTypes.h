#pragma once

namespace Rhi
{
	enum class GsInputPrimitiveTopology
	{
		POINTS = 0x0000,	// List of point primitives
		LINES = 0x0001,	// List of line or line strip primitives
		LINES_ADJACENCY = 0x000A,	// List of line with adjacency or line strip with adjacency primitives
		TRIANGLES = 0x0004,	// List of triangle or triangle strip primitives
		TRIANGLES_ADJACENCY = 0x000C	// List of triangle with adjacency or triangle strip with adjacency primitives
	};

	enum class GsOutputPrimitiveTopology
	{
		POINTS = 0x0000,	// A list of point primitives
		LINES = 0x0001,	// A list of line primitives
		TRIANGLES_STRIP = 0x0005	// A triangle strip primitives
	};

	class ShaderBytecode final
	{
	public:
		inline ~ShaderBytecode()
		{
			delete[] mBytecode;
		}

		[[nodiscard]] inline uint32_t getNumberOfBytes() const
		{
			return mNumberOfBytes;
		}

		[[nodiscard]] inline const uint8_t* getBytecode() const
		{
			return mBytecode;
		}

		inline void setBytecodeCopy(uint32_t numberOfBytes, uint8_t* bytecode)
		{
			delete[] mBytecode;
			mNumberOfBytes = numberOfBytes;
			mBytecode = new uint8_t[mNumberOfBytes];
			memcpy(mBytecode, bytecode, mNumberOfBytes);
		}

	private:
		uint32_t mNumberOfBytes = 0; // Number of bytes in the bytecode
		uint8_t *mBytecode = nullptr; // Shader bytecode, can be a null pointer
	};

	struct ShaderSourceCode final
	{
		const char* sourceCode = nullptr;	// Shader ASCII source code, if null pointer or empty string a null pointer will be returned
		const char* profile = nullptr;	// Shader ASCII profile to use, if null pointer or empty string, a default profile will be used which usually tries to use the best available profile that runs on most hardware (Examples: "glslf", "arbfp1", "ps_3_0")
		const char* arguments = nullptr;	// Optional shader compiler ASCII arguments, can be a null pointer or empty string
		const char* entry = nullptr;	// ASCII entry point, if null pointer or empty string, "main" is used as default

		inline ShaderSourceCode(const char* _sourceCode) : sourceCode(_sourceCode) {}
	};
} // namespace Rhi