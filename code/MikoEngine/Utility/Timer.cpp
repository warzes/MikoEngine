#include "stdafx.h"
#include "Timer.h"
//-----------------------------------------------------------------------------
Timer::Timer()
{
#if SE_PLATFORM_WINDOWS
	QueryPerformanceFrequency(&m_frequency);
	m_startCount.QuadPart = 0;
	m_endCount.QuadPart = 0;
#else
	m_startCount.tv_sec = m_startCount.tv_usec = 0;
	m_endCount.tv_sec = m_endCount.tv_usec = 0;
#endif // SE_PLATFORM_*
}
//-----------------------------------------------------------------------------
void Timer::Start()
{
	m_stopped = 0;
#if SE_PLATFORM_WINDOWS
	QueryPerformanceCounter(&m_startCount);
#else
	gettimeofday(&m_startCount, nullptr);
#endif // SE_PLATFORM_*
}
//-----------------------------------------------------------------------------
void Timer::Stop()
{
	m_stopped = 1;
#if SE_PLATFORM_WINDOWS
	QueryPerformanceCounter(&m_endCount);
#else
	gettimeofday(&m_endCount, nullptr);
#endif // SE_PLATFORM_*
}
//-----------------------------------------------------------------------------
double Timer::ElapsedTime()
{
	return ElapsedTimeSec();
}
//-----------------------------------------------------------------------------
double Timer::ElapsedTimeSec()
{
	return ElapsedTimeMicrosec() * 0.000001;
}
//-----------------------------------------------------------------------------
double Timer::ElapsedTimeMilisec()
{
	return ElapsedTimeMicrosec() * 0.001;
}
//-----------------------------------------------------------------------------
double Timer::ElapsedTimeMicrosec()
{
#if SE_PLATFORM_WINDOWS
	if ( !m_stopped )
		QueryPerformanceCounter(&m_endCount);

	m_startTimeMicrosec = m_startCount.QuadPart * (1000000.0 / m_frequency.QuadPart);
	m_endTimeMicrosec = m_endCount.QuadPart * (1000000.0 / m_frequency.QuadPart);
#else
	if ( !m_stopped )
		gettimeofday(&m_endCount, nullptr);

	m_startTimeMicrosec = (m_startCount.tv_sec * 1000000.0) + m_startCount.tv_usec;
	m_endTimeMicrosec = (m_endCount.tv_sec * 1000000.0) + m_endCount.tv_usec;
#endif // SE_PLATFORM_*

	return m_endTimeMicrosec - m_startTimeMicrosec;
}
//-----------------------------------------------------------------------------