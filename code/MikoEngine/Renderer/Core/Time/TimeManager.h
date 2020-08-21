#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "Renderer/Core/Manager.h"
#include "Renderer/Core/Time/Stopwatch.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Time manager
	*/
	class TimeManager final : public Manager
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		inline TimeManager() :
			mPastSecondsSinceLastFrame(std::numeric_limits<float>::min()),	// Don't initialize with zero or time advancing enforcement asserts will get more complicated
			mGlobalTimeInSeconds(0.0f),
			mNumberOfRenderedFrames(0)
		{
			// Nothing here
		}

		inline ~TimeManager()
		{
			// Nothing here
		}

		[[nodiscard]] inline float getPastSecondsSinceLastFrame() const
		{
			return mPastSecondsSinceLastFrame;
		}

		[[nodiscard]] inline float getGlobalTimeInSeconds() const
		{
			return mGlobalTimeInSeconds;
		}

		[[nodiscard]] inline uint64_t getNumberOfRenderedFrames() const
		{
			return mNumberOfRenderedFrames;
		}

		/**
		*  @brief
		*    Time manager update
		*
		*  @note
		*    - Call this once per frame
		*/
		void update();


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		explicit TimeManager(const TimeManager&) = delete;
		TimeManager& operator=(const TimeManager&) = delete;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		Stopwatch mStopwatch;
		float	  mPastSecondsSinceLastFrame;
		float	  mGlobalTimeInSeconds;
		uint64_t  mNumberOfRenderedFrames;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer
