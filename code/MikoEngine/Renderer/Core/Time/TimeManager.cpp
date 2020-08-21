#include "stdafx.h"
#include "Renderer/Core/Time/TimeManager.h"

namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	void TimeManager::update()
	{
		// Stop the stopwatch and get the past milliseconds
		mStopwatch.stop();
		mPastSecondsSinceLastFrame = mStopwatch.getSeconds();
		if (mPastSecondsSinceLastFrame <= 0.0f)
		{
			// Don't allow zero or time advancing enforcement asserts will get more complicated
			mPastSecondsSinceLastFrame = std::numeric_limits<float>::min();
		}
		else if (mPastSecondsSinceLastFrame > 0.06f)
		{
			// No one likes huge time jumps
			mPastSecondsSinceLastFrame = 0.06f;
		}
		mGlobalTimeInSeconds += mPastSecondsSinceLastFrame;	// TODO(co) Add some kind of wrapping to avoid that issues with a too huge global time can come up?
		++mNumberOfRenderedFrames;

		// Start the stopwatch
		mStopwatch.start();
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer
