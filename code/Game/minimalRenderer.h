#pragma once

#include <MikoEngine/MikoHeader.h>
#include "MikoEngine/Renderer/Resource/IResourceListener.h"
#include "MikoEngine/Core/File/IFileManager.h"
#include "MikoEngine/Renderer/RendererInstance.h"
#include "MikoEngine/Renderer/Core/IProfiler.h"

class minimalRenderer : public Application
{
public:
    bool init(int argc, const char* argv[]) override;
    void update(double delta) override;
    void shutdown() override;

    ApplicationSetting intial_app_settings() override;

    void window_resized(int width, int height) override;

private:
    void fillCommandBuffer();

    Renderer::IRenderer* getRenderer() const;

    IFileManager*		mFileManager;
#ifdef RENDERER_PROFILER
    Renderer::IProfiler*	mProfiler;			///< Profiler instance, can be a null pointer
#endif
    Renderer::Context*			mRendererContext;	///< Renderer context instance, can be a null pointer
    Renderer::RendererInstance* mRendererInstance;	///< Renderer instance, can be a null pointer
#ifdef RENDERER_GRAPHICS_DEBUGGER
    Renderer::IGraphicsDebugger* mGraphicsDebugger;	///< Graphics debugger instance, can be a null pointer
#endif
};