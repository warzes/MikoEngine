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
	class DebugGuiManagerLinux final : public DebugGuiManager
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class RendererImpl;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		void onWindowResize(uint32_t width, uint32_t heigth);
		void onKeyInput(uint32_t keySym, char character, bool pressed);
		void onMouseMoveInput(int x, int y);
		void onMouseButtonInput(uint32_t button, bool pressed);
		void onMouseWheelInput(bool scrollUp);


	//[-------------------------------------------------------]
	//[ Protected virtual Renderer::DebugGuiManager methods   ]
	//[-------------------------------------------------------]
	protected:
		virtual void initializeImGuiKeyMap() override;
		virtual void onNewFrame(Rhi::IRenderTarget& renderTarget) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		inline explicit DebugGuiManagerLinux(IRenderer& renderer) :
			DebugGuiManager(renderer),
			mWindowWidth(0),
			mWindowHeigth(0),
			mTime(0)
		{
			// Nothing here
		}

		inline virtual ~DebugGuiManagerLinux() override
		{
			// Nothing here
		}

		explicit DebugGuiManagerLinux(const DebugGuiManagerLinux&) = delete;
		DebugGuiManagerLinux& operator=(const DebugGuiManagerLinux&) = delete;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint32_t mWindowWidth;
		uint32_t mWindowHeigth;
		uint64_t mTime;	//< Holds the time in microseconds


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer
