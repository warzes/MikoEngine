#pragma once

namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Stopwatch
	*
	*  @verbatim
	*    Usage example:
	*
	*    // Start the stopwatch
	*    Stopwatch stopwatch(true);
	*
	*    // Do some stuff
	*
	*    // Measure elapsed time
	*    float elapsedSeconds = stopwatch.getSeconds();
	*  @endverbatim
	*/
	class Stopwatch final
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		inline Stopwatch() :
			mRunning(false),
			mStart(0),
			mStop(0)
		{
			// Nothing here
		}

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] startAtOnce
		*    If this parameter is "true", the stopwatch is started automatically at once
		*/
		inline explicit Stopwatch(bool startAtOnce) :
			mRunning(false),
			mStart(0),
			mStop(0)
		{
			if (startAtOnce)
			{
				start();
			}
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline ~Stopwatch()
		{
			// Nothing here
		}

		/**
		*  @brief
		*    Start the stopwatch
		*
		*  @note
		*    - If the stopwatch is already running it's restarted
		*/
		inline void start()
		{
			mRunning = true;
			mStart   = getSystemMicroseconds();
		}

		/**
		*  @brief
		*    Stop the stopwatch
		*
		*  @return
		*    The elapsed time in microseconds since "start()"
		*
		*  @note
		*    - Often it's adequate to just request the past time using
		*      e.g. "getMilliseconds()" and not explicitly stopping the stopwatch
		*/
		inline std::time_t stop()
		{
			// Is the stopwatch currently running?
			if (mRunning)
			{
				mStop    = getSystemMicroseconds();
				mRunning = false;
				return mStop - mStart;
			}
			else
			{
				return 0;
			}
		}

		/**
		*  @brief
		*    Return the number of weeks since the stopwatch was started
		*
		*  @return
		*    Number of weeks elapsed since the stopwatch was started
		*/
		[[nodiscard]] inline float getWeeks() const
		{
			return getDays() / 7.0f;
		}

		/**
		*  @brief
		*    Return the number of days since the stopwatch was started
		*
		*  @return
		*    Number of days elapsed since the stopwatch was started
		*/
		[[nodiscard]] inline float getDays() const
		{
			return getHours() / 24.0f;
		}

		/**
		*  @brief
		*    Return the number of hours since the stopwatch was started
		*
		*  @return
		*    Number of hours elapsed since the stopwatch was started
		*/
		[[nodiscard]] inline float getHours() const
		{
			return getMinutes() / 60.0f;
		}

		/**
		*  @brief
		*    Return the number of minutes since the stopwatch was started
		*
		*  @return
		*    Number of minutes elapsed since the stopwatch was started
		*/
		[[nodiscard]] inline float getMinutes() const
		{
			return getSeconds() / 60.0f;
		}

		/**
		*  @brief
		*    Return the number of seconds since the stopwatch was started
		*
		*  @return
		*    Number of seconds elapsed since the stopwatch was started
		*/
		[[nodiscard]] inline float getSeconds() const
		{
			return getMilliseconds() / 1000.0f;
		}

		/**
		*  @brief
		*    Return the number of milliseconds since the stopwatch was started
		*
		*  @return
		*    Number of milliseconds elapsed since the stopwatch was started
		*/
		[[nodiscard]] inline float getMilliseconds() const
		{
			return getMicroseconds() / 1000.0f;
		}

		/**
		*  @brief
		*    Retrieve the number of microseconds since the stopwatch was started
		*
		*  @return
		*    Number of microseconds elapsed since the stopwatch was started
		*/
		[[nodiscard]] inline std::time_t getMicroseconds() const
		{
			return mRunning ? (getSystemMicroseconds() - mStart) : (mStop - mStart);
		}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Retrieve the number of microseconds since the system was started
		*
		*  @return
		*    Number of microseconds elapsed since the system was started
		*/
		[[nodiscard]] std::time_t getSystemMicroseconds() const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		bool		mRunning;	// Is the stopwatch currently running?
		std::time_t mStart;		// Stopwatch start time (microseconds)
		std::time_t	mStop;		// Stopwatch stop time (microseconds)


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer
