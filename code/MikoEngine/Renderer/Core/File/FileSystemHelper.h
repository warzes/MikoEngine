// TODO(co) The C++17 filesystem is still experimental and hence not handled in an uniform way cross platform. Until C++17 has been released on all targeted platforms, we need to use those helper.

#pragma once



namespace std_filesystem = std::filesystem;

namespace Renderer
{
	class FileSystemHelper final
	{
	public:
		[[nodiscard]] static std_filesystem::path lexicallyNormal(const std_filesystem::path& path);

	private:
		FileSystemHelper(const FileSystemHelper&) = delete;
		FileSystemHelper& operator=(const FileSystemHelper&) = delete;
	};
} // Renderer