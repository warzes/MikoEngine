#pragma once

namespace Renderer
{
	class Loader
	{
	protected:
		inline Loader() {}
		inline ~Loader() {}
		explicit Loader(const Loader&) = delete;
		Loader& operator=(const Loader&) = delete;
	};
} // Renderer