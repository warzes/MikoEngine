#pragma once

#if SE_OPENGL

#include "Render/IRenderer.h"

class OpenGLRenderer final : public IRenderer
{
public:
	OpenGLRenderer();
	~OpenGLRenderer() final;

protected:
	inline virtual void selfDestruct() final
	{
		delete this;
	}

private:
	explicit OpenGLRenderer(const OpenGLRenderer&) = delete;
	OpenGLRenderer& operator =(const OpenGLRenderer&) = delete;
};

#endif // SE_OPENGL