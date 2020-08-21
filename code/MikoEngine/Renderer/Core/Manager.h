#pragma once

namespace Renderer
{
	class Manager
	{
	protected:
		inline Manager() {}
		inline ~Manager() {}
		explicit Manager(const Manager&) = delete;
		Manager& operator=(const Manager&) = delete;
	};
} // Renderer