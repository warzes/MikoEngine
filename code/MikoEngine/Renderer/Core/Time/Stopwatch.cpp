#include "stdafx.h"
#include "Renderer/Core/Time/Stopwatch.h"

namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	std::time_t Stopwatch::getSystemMicroseconds() const
	{
		#ifdef _WIN32
			// Frequency of the performance counter
			static LARGE_INTEGER performanceFrequency;
			static bool performanceFrequencyInitialized = false;
			if (!performanceFrequencyInitialized)
			{
				::QueryPerformanceFrequency(&performanceFrequency);
				performanceFrequencyInitialized = true;
			}

			// Get past time
			LARGE_INTEGER curTime;
			::QueryPerformanceCounter(&curTime);
			double newTicks = static_cast<double>(curTime.QuadPart);

			// Scale by 1000000 in order to get microsecond precision
			newTicks *= static_cast<double>(1000000.0) / static_cast<double>(performanceFrequency.QuadPart);

			// Return past time
			return static_cast<std::time_t>(newTicks);
		#elif defined LINUX
			struct timeval now;
			gettimeofday(&now, nullptr);
			return static_cast<std::time_t>(now.tv_sec * 1000000 + now.tv_usec);
		#else
			#error "Unsupported platform"
		#endif
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer
