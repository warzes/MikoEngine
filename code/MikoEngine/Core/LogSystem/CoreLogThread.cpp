#include "stdafx.h"
#if !SE_PLATFORM_EMSCRIPTEN
#include "CoreLogThread.h"
//-----------------------------------------------------------------------------
CoreLogThread::CoreLogThread(logFlushImpl *flushFunc)
	: m_logFlushImpl(flushFunc)
{
}
//-----------------------------------------------------------------------------
CoreLogThread::~CoreLogThread()
{
	close();
}
//-----------------------------------------------------------------------------
void CoreLogThread::print(const std::string &str, const LogVerbosity verbosity) const
{
	std::unique_lock<std::mutex> lock(m_queueMutex);
	m_commandQueue.push(command(command::Type::LogString, verbosity, str));
	lock.unlock();
	m_queueCondition.notify_all();
}
//-----------------------------------------------------------------------------
void CoreLogThread::LogLoop()
{
	for ( ;;)
	{
		std::unique_lock<std::mutex> lock(m_queueMutex);
		while ( m_commandQueue.empty() )
			m_queueCondition.wait(lock);
		auto command = std::move(m_commandQueue.front());
		m_commandQueue.pop();
		lock.unlock();

		if ( command.type == command::Type::LogString )
			m_logFlushImpl(command.str, command.verbosity);
		else if ( command.type == command::Type::Quit )
			break;
	}
}
//-----------------------------------------------------------------------------
void CoreLogThread::close()
{
	if ( m_isClose ) return;

	std::unique_lock<std::mutex> lock(m_queueMutex);
	m_commandQueue.push(command(command::Type::Quit));
	lock.unlock();
	m_queueCondition.notify_all();

	if ( m_thread.joinable() ) m_thread.join();
	m_isClose = true;
}
//-----------------------------------------------------------------------------
#endif