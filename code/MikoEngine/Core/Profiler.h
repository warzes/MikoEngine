#pragma once

#include "RenderHeader.h"

#if SE_VULKAN
#	define SE_SCOPED_SAMPLE(name, cmd_buf) profiler::ScopedProfile __FILE__##__LINE__(name, cmd_buf)
#else
#	define SE_SCOPED_SAMPLE(name) profiler::ScopedProfile __FILE__##__LINE__(name)
#endif

namespace profiler
{
	struct ScopedProfile
	{
		ScopedProfile(std::string name
#if SE_VULKAN
			,
			vk::CommandBuffer::Ptr cmd_buf
#endif
		);
		~ScopedProfile();

#if SE_VULKAN
		vk::CommandBuffer::Ptr m_cmd_buf;
#endif
		std::string m_name;
	};

	extern void initialize(
#if SE_VULKAN
		vk::Backend::Ptr backend
#endif
	);
	extern void shutdown();
	extern void begin_sample(std::string name
#if SE_VULKAN
		,
		vk::CommandBuffer::Ptr cmd_buf
#endif
	);
	extern void end_sample(std::string name
#if SE_VULKAN
		,
		vk::CommandBuffer::Ptr cmd_buf
#endif
	);
	extern void begin_frame();
	extern void end_frame();

	extern void ui();

}; // namespace profiler