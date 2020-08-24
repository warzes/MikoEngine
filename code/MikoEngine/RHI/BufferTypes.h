#pragma once

namespace Rhi
{
	struct BufferFlag final
	{
		enum Enum
		{
			UNORDERED_ACCESS = 1 << 0, // This buffer can be used for unordered access which is needed for compute shader read/write buffers (when using Direct3D 11 a unordered access view (UAV) will be generated)
			SHADER_RESOURCE  = 1 << 1  // This buffer can be used as shader resource (when using Direct3D 11 a shader resource view (SRV) will be generated)
		};
	};

	struct IndirectBufferFlag final
	{
		enum Enum
		{
			UNORDERED_ACCESS       = 1 << 0, // This indirect buffer can be used for unordered access which is needed for compute shader read/write indirect buffers (when using Direct3D 11 a unordered access view (UAV) will be generated)
			SHADER_RESOURCE       = 1 << 1, // This indirect buffer can be used as shader resource (when using Direct3D 11 a shader resource view (SRV) will be generated)
			DRAW_ARGUMENTS        = 1 << 2, // This indirect buffer contains "Rhi::DrawArguments" as elements, if "Rhi::IndirectBufferFlag::DRAW_ARGUMENTS" is set it's invalid to have "Rhi::IndirectBufferFlag::DRAW_INDEXED_ARGUMENTS" set as well
			DRAW_INDEXED_ARGUMENTS = 1 << 3 // This indirect buffer contains "Rhi::DrawIndexedArguments" as elements, if "Rhi::IndirectBufferFlag::DRAW_ARGUMENTS" is set it's invalid to have "Rhi::IndirectBufferFlag::DRAW_INDEXED_ARGUMENTS" set as well
		};
	};

	/**
	*  @brief
	*    Buffer usage indication
	*
	*  @note
	*    - Direct3D and OpenGL ES 3 have less fine granular usage settings, in this case the usage will be mapped to the closed match
	*    - Original OpenGL "GL_ARB_vertex_buffer_object" extension comments from http://www.opengl.org/registry/specs/ARB/vertex_buffer_object.txt are used in here
	*    - These constants directly map to "GL_ARB_vertex_buffer_object" and OpenGL ES 3 constants, do not change them
	*    - Most common usages: "STREAM_DRAW", "STATIC_DRAW" and "DYNAMIC_DRAW"
	*/
	enum class BufferUsage
	{
		STREAM_DRAW = 0x88E0,	// The data store contents will be specified once by the application, and used at most a few times as the source of a OpenGL (drawing) command. (also exists in OpenGL ES 3)
		STREAM_READ = 0x88E1,	// The data store contents will be specified once by reading data from the OpenGL, and queried at most a few times by the application.
		STREAM_COPY = 0x88E2,	// The data store contents will be specified once by reading data from the OpenGL, and used at most a few times as the source of a OpenGL (drawing) command.
		STATIC_DRAW = 0x88E4,	// The data store contents will be specified once by the application, and used many times as the source for OpenGL (drawing) commands. (also exists in OpenGL ES 3)
		STATIC_READ = 0x88E5,	// The data store contents will be specified once by reading data from the OpenGL, and queried many times by the application.
		STATIC_COPY = 0x88E6,	// The data store contents will be specified once by reading data from the OpenGL, and used many times as the source for OpenGL (drawing) commands.
		DYNAMIC_DRAW = 0x88E8,	// The data store contents will be respecified repeatedly by the application, and used many times as the source for OpenGL (drawing) commands. (also exists in OpenGL ES 3)
		DYNAMIC_READ = 0x88E9,	// The data store contents will be respecified repeatedly by reading data from the OpenGL, and queried many times by the application.
		DYNAMIC_COPY = 0x88EA	// The data store contents will be respecified repeatedly by reading data from the OpenGL, and used many times as the source for OpenGL (drawing) commands.
	};
} // namespace Rhi