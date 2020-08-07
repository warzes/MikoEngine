#pragma once

#include "Core/MemoryPtr.h"

class IRenderer : public RefCount<IRenderer>
{
public:
	IRenderer() = default;
	virtual ~IRenderer() = default;

	[[nodiscard]] virtual const char* GetName() const = 0;

protected:

private:
	explicit IRenderer(const IRenderer&) = delete;
	IRenderer& operator =(const IRenderer&) = delete;
};

typedef SmartRefCount<IRenderer> IRendererPtr;