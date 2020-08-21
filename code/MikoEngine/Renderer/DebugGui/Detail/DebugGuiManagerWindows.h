#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "Renderer/DebugGui/DebugGuiManager.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	class DebugGuiManagerWindows final : public DebugGuiManager
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class RendererImpl;


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Microsoft Windows callback method to be called from the outside for input handling
		*/
		static LRESULT wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


	//[-------------------------------------------------------]
	//[ Protected virtual Renderer::DebugGuiManager methods   ]
	//[-------------------------------------------------------]
	protected:
		virtual void initializeImGuiKeyMap() override;
		virtual void startup() override;
		virtual void onNewFrame(Rhi::IRenderTarget& renderTarget) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		inline explicit DebugGuiManagerWindows(IRenderer& renderer) :
			DebugGuiManager(renderer),
			mTime(0),
			mTicksPerSecond(0)
		{
			// Nothing here
		}

		inline virtual ~DebugGuiManagerWindows() override
		{
			// Nothing here
		}

		explicit DebugGuiManagerWindows(const DebugGuiManagerWindows&) = delete;
		DebugGuiManagerWindows& operator=(const DebugGuiManagerWindows&) = delete;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		INT64 mTime;
		INT64 mTicksPerSecond;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer
