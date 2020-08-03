#pragma once

// TODO: TIME_IMPL_STL - Timer реализован через chrono (возможно переименовать дефайн)

class Timer
{
public:
	Timer();

	void Start();
	void Stop();
	double ElapsedTime();
	double ElapsedTimeSec();
	double ElapsedTimeMilisec();
	double ElapsedTimeMicrosec();
private:
	double m_startTimeMicrosec = 0.0;
	double m_endTimeMicrosec = 0.0;
	int m_stopped = 0;

#if SE_PLATFORM_WINDOWS
	LARGE_INTEGER m_frequency;
	LARGE_INTEGER m_startCount;
	LARGE_INTEGER m_endCount;
#else
	timeval m_startCount;
	timeval m_endCount;
#endif // SE_PLATFORM_*
};