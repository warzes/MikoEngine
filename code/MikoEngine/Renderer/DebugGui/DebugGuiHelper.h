#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]



//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Renderer
{
	class Transform;
	class CameraSceneItem;
	class SkeletonMeshSceneItem;
	class CompositorWorkspaceInstance;
}


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
	*    Debug GUI helper generating ImGui ( https://github.com/ocornut/imgui ) commands
	*/
	class DebugGuiHelper final
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class DebugGuiManager;	// Calls "Renderer::DebugGuiHelper::beginFrame()"


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		enum class GizmoOperation
		{
			TRANSLATE,
			ROTATE,
			SCALE
		};
		enum class GizmoMode
		{
			LOCAL,
			WORLD
		};
		struct GizmoSettings final
		{
			GizmoOperation	currentGizmoOperation = GizmoOperation::TRANSLATE;
			GizmoMode		currentGizmoMode	  = GizmoMode::WORLD;
			bool			useSnap				  = false;
			float			snap[3]					{1.0f, 1.0f, 1.0f};
		};


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		static void drawText(const char* text, float x, float y, bool drawBackground = true);
		static void drawGizmo(const CameraSceneItem& cameraSceneItem, GizmoSettings& gizmoSettings, Transform& transform);	// Using "ImGuizmo" ( https://github.com/CedricGuillemet/ImGuizmo )
		static void drawSkeleton(const CameraSceneItem& cameraSceneItem, const SkeletonMeshSceneItem& skeletonMeshSceneItem);
		static void drawGrid(const CameraSceneItem& cameraSceneItem, float cellSize = 1.0f, double yPosition = 0.0f);


	//[-------------------------------------------------------]
	//[ Private static methods                                ]
	//[-------------------------------------------------------]
	private:
		inline static void beginFrame()
		{
			// Reset the draw text counter
			mDrawTextCounter = 0;
		}

		static void drawMetricsWindow(bool& open, CompositorWorkspaceInstance* compositorWorkspaceInstance);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		DebugGuiHelper() = delete;
		~DebugGuiHelper() = delete;
		explicit DebugGuiHelper(const DebugGuiHelper&) = delete;
		DebugGuiHelper& operator=(const DebugGuiHelper&) = delete;


	//[-------------------------------------------------------]
	//[ Private static data                                   ]
	//[-------------------------------------------------------]
	private:
		static uint32_t mDrawTextCounter;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer
