#pragma once

#include "Renderer/Core/File/IFile.h"
#include "Renderer/Core/File/IFileManager.h"
#include "Renderer/Core/File/FileSystemHelper.h"

#include <android/asset_manager.h>

namespace
{
	namespace detail
	{
		static constexpr const char* ANDROID_LOCAL_DATA_MOUNT_POINT = "LocalData";

		[[nodiscard]] bool doesFileExist(AAssetManager& aAssetManager, Renderer::VirtualFilename virtualFilename)
		{
			AAsset* aAsset = AAssetManager_open(&aAssetManager, virtualFilename, AASSET_MODE_STREAMING);
			if ( nullptr != aAsset )
			{
				AAsset_close(aAsset);

				// File exists
				return true;
			}

			// File doesn't exist
			return false;
		}
		class AndroidFile : public Renderer::IFile
		{
		public:
			inline AndroidFile()
			{
				// Nothing here
			}

			inline virtual ~AndroidFile() override
			{
				// Nothing here
			}

			[[nodiscard]] virtual bool isInvalid() const = 0;

		protected:
			explicit AndroidFile(const AndroidFile&) = delete;
			AndroidFile& operator=(const AndroidFile&) = delete;
		};

		class AndroidReadFile final : public AndroidFile
		{
		public:
			inline AndroidReadFile(AAssetManager& aAssetManager, const std::string& absoluteFilename) :
				mAAsset(AAssetManager_open(&aAssetManager, absoluteFilename.c_str(), AASSET_MODE_STREAMING))
			{
				RHI_ASSERT(nullptr != mAAsset, "Failed to open Android file for reading")
			}

			inline virtual ~AndroidReadFile() override
			{
				if ( nullptr != mAAsset )
				{
					AAsset_close(mAAsset);
				}
			}

		public:
			[[nodiscard]] inline virtual bool isInvalid() const override
			{
				return (nullptr == mAAsset);
			}

			[[nodiscard]] inline virtual size_t getNumberOfBytes() override
			{
				RHI_ASSERT(nullptr != mAAsset, "Invalid Android file access")
#if SE_ARCH_64BIT
					return AAsset_getLength64(mAAsset);
#else
					return AAsset_getLength(mAAsset);
#endif
			}

			inline virtual void read(void* destinationBuffer, size_t numberOfBytes) override
			{
				RHI_ASSERT(nullptr != destinationBuffer, "Letting a file read into a null destination buffer is not allowed")
					RHI_ASSERT(0 != numberOfBytes, "Letting a file read zero bytes is not allowed")
					RHI_ASSERT(nullptr != mAAsset, "Invalid Android file access")
					[[maybe_unused]] const int numberOfReadBytes = AAsset_read(mAAsset, destinationBuffer, numberOfBytes);
				RHI_ASSERT(numberOfReadBytes == numberOfBytes, "Failed to read the requested number of bytes from the Android file")
			}

			inline virtual void skip(size_t numberOfBytes) override
			{
				RHI_ASSERT(0 != numberOfBytes, "Letting a file skip zero bytes is not allowed")
					RHI_ASSERT(nullptr != mAAsset, "Invalid Android file access")
#if SE_ARCH_64BIT
					[[maybe_unused]] const off64_t newPosition = AAsset_seek64(mAAsset, numberOfBytes, SEEK_CUR);
#else
					[[maybe_unused]] const off_t newPosition = AAsset_seek(mAAsset, numberOfBytes, SEEK_CUR);
#endif
				RHI_ASSERT(-1 != newPosition, "Failed to skip the requested number of bytes from the Android file")
			}

			inline virtual void write([[maybe_unused]] const void* sourceBuffer, [[maybe_unused]] size_t numberOfBytes) override
			{
				RHI_ASSERT(nullptr != sourceBuffer, "Letting a file write from a null source buffer is not allowed")
					RHI_ASSERT(0 != numberOfBytes, "Letting a file write zero bytes is not allowed")
					RHI_ASSERT(nullptr != mAAsset, "Invalid Android file access")
					RHI_ASSERT(false, "File write method not supported by the Android implementation")
			}

		protected:
			explicit AndroidReadFile(const AndroidReadFile&) = delete;
			AndroidReadFile& operator=(const AndroidReadFile&) = delete;

		private:
			AAsset* mAAsset;	// Android asset, might be a null pointer
		};
	} // detail
}

namespace Renderer
{
	/**
	*  @brief
	*    Android file manager implementation class one can use
	*
	*  @note
	*    - Designed to be instanced and used inside a single C++ file
	*    - Primarily for renderer toolkit with more relaxed write access
	*/
	class AndroidFileManager final : public IFileManager
	{
	public:
		inline AndroidFileManager(Rhi::ILog& log, Rhi::IAssert& assert, DefaultAllocator& allocator, const std::string& absoluteRootDirectory, AAssetManager& aAssetManager) :
			IFileManager(absoluteRootDirectory),
			mLog(log),
			mAssert(assert),
			mAllocator(allocator),
			mAAssetManager(aAssetManager)
		{
			// Setup local data mount point
			mAbsoluteBaseDirectory.push_back(absoluteRootDirectory.c_str());
			// createDirectories(::detail::ANDROID_LOCAL_DATA_MOUNT_POINT);	// Not supported on Android
			mountDirectory((absoluteRootDirectory + '/' + ::detail::ANDROID_LOCAL_DATA_MOUNT_POINT).c_str(), ::detail::ANDROID_LOCAL_DATA_MOUNT_POINT);
		}

		inline virtual ~AndroidFileManager() override
		{
			RHI_ASSERT(0 == mNumberOfCurrentlyOpenedFiles, "File leak detected, not all opened files were closed")
		}

	public:
		[[nodiscard]] inline virtual const char* getLocalDataMountPoint() const override
		{
			return ::detail::ANDROID_LOCAL_DATA_MOUNT_POINT;
		}

		[[nodiscard]] inline virtual const char* getMountPoint(const char* mountPoint) const override
		{
			RHI_ASSERT(nullptr != mountPoint, "Invalid mount point")
				const MountedDirectories::const_iterator mountedDirectoriesIterator = mMountedDirectories.find(mountPoint);
			if ( mMountedDirectories.cend() != mountedDirectoriesIterator )
			{
				const AbsoluteDirectoryNames& absoluteDirectoryNames = mountedDirectoriesIterator->second;
				return absoluteDirectoryNames.empty() ? nullptr : absoluteDirectoryNames[0].c_str();
			}
			else
			{
				// Unknown mount point
				return nullptr;
			}
		}

		inline virtual bool mountDirectory(AbsoluteDirectoryName absoluteDirectoryName, const char* mountPoint, bool appendToPath = false) override
		{
			// Sanity check
			RHI_ASSERT(nullptr != absoluteDirectoryName, "Invalid absolute directory name")
				RHI_ASSERT(nullptr != mountPoint, "Invalid mount point")
#if SE_DEBUG
				// Additional sanity check: The same absolute directory name shouldn't be added to too different mount points
				for ( const auto& pair : mMountedDirectories )
				{
					if ( pair.first != mountPoint )
					{
						const AbsoluteDirectoryNames& absoluteDirectoryNames = pair.second;
						RHI_ASSERT(absoluteDirectoryNames.cend() == std::find(absoluteDirectoryNames.begin(), absoluteDirectoryNames.end(), absoluteDirectoryName), "The same absolute directory name shouldn't be added to too different Android mount points")
					}
				}
#endif

			// Mount directory
			MountedDirectories::iterator mountedDirectoriesIterator = mMountedDirectories.find(mountPoint);
			if ( mMountedDirectories.cend() == mountedDirectoriesIterator )
			{
				// The mount point is unknown so far, register it
				mMountedDirectories.emplace(mountPoint, AbsoluteDirectoryNames{ absoluteDirectoryName });
			}
			else
			{
				// The mount point is already known, append or prepend?
				AbsoluteDirectoryNames& absoluteDirectoryNames = mountedDirectoriesIterator->second;
				AbsoluteDirectoryNames::const_iterator absoluteDirectoryNamesIterator = std::find(absoluteDirectoryNames.begin(), absoluteDirectoryNames.end(), absoluteDirectoryName);
				if ( absoluteDirectoryNames.cend() == absoluteDirectoryNamesIterator )
				{
					if ( appendToPath )
					{
						// Append
						absoluteDirectoryNames.push_back(absoluteDirectoryName);
					}
					else
					{
						// Prepend
						absoluteDirectoryNames.insert(absoluteDirectoryNames.begin(), absoluteDirectoryName);
					}
				}
				else
				{
					RHI_ASSERT(false, "Duplicate absolute Android directory name detected, this situation should be avoided by the caller")
				}
			}

			// Done
			return true;
		}

		[[nodiscard]] inline virtual bool doesFileExist(VirtualFilename virtualFilename) const override
		{
			return !mapVirtualToAbsoluteFilename(FileMode::READ, virtualFilename).empty();
		}

		inline virtual void enumerateFiles(VirtualDirectoryName virtualDirectoryName, EnumerationMode enumerationMode, std::vector<std::string>& virtualFilenames) const override
		{
			// TODO(co) "AAssetDir_getNextFileName()" filters out directories, so we currently can just list files without additional implementation efforts
			//          -> See https://android.googlesource.com/platform/frameworks/base.git/+/1cadc07dd1e3711fb1e57548038e3026682c5ef8/native/android/asset_manager.cpp#135
			if ( EnumerationMode::DIRECTORIES != enumerationMode )
			{
				std::string mountPoint;
				const std::string absoluteDirectoryName = mapVirtualToAbsoluteFilenameAndMountPoint(FileMode::READ, virtualDirectoryName, mountPoint);
				if ( !absoluteDirectoryName.empty() )
				{
					AAssetDir* aAssetDir = AAssetManager_openDir(&mAAssetManager, absoluteDirectoryName.c_str());
					if ( nullptr != aAssetDir )
					{
						const size_t numberOfSkippedBytes = absoluteDirectoryName.length() + 1;	// +1 for '/'-slash at the end of the absolute directory name
						const char* filename = AAssetDir_getNextFileName(aAssetDir);
						while ( nullptr != filename )
						{
							virtualFilenames.push_back(mountPoint + '/' + std::string(filename).erase(0, numberOfSkippedBytes));

							// Next, please
							filename = AAssetDir_getNextFileName(aAssetDir);
						}
						AAssetDir_close(aAssetDir);
					}
				}
			}
		}

		[[nodiscard]] inline virtual std::string mapVirtualToAbsoluteFilename(FileMode fileMode, VirtualFilename virtualFilename) const override
		{
			std::string mountPoint;
			return mapVirtualToAbsoluteFilenameAndMountPoint(fileMode, virtualFilename, mountPoint);
		}

		[[nodiscard]] inline virtual int64_t getLastModificationTime(VirtualFilename) const override
		{
			RHI_ASSERT(false, "Renderer::IFileManager::getLastModificationTime() isn't supported on Android")
				return -1;
		}

		[[nodiscard]] inline virtual int64_t getFileSize(VirtualFilename virtualFilename) const override
		{
			int64_t fileSize = -1;
			const std::string absoluteFilename = mapVirtualToAbsoluteFilename(FileMode::READ, virtualFilename);
			if ( !absoluteFilename.empty() )
			{
				AAsset* aAsset = AAssetManager_open(&mAAssetManager, absoluteFilename.c_str(), AASSET_MODE_STREAMING);
				if ( nullptr != aAsset )
				{
#if SE_ARCH_64BIT
					fileSize = AAsset_getLength64(aAsset);
#else
					fileSize = AAsset_getLength(aAsset);
#endif
					AAsset_close(aAsset);
				}
			}
			return fileSize;
		}

		inline virtual bool createDirectories([[maybe_unused]] VirtualDirectoryName virtualDirectoryName) const override
		{
			// Sanity check
			RHI_ASSERT(nullptr != virtualDirectoryName, "Invalid virtual directory name")

				// Error, not supported on Android
				RHI_ASSERT(false, "\"Renderer::IFileManager::createDirectories()\" isn't supported on Android")
				return false;
		}

		[[nodiscard]] inline virtual IFile* openFile(FileMode fileMode, VirtualFilename virtualFilename) const override
		{
			::detail::AndroidFile* file = nullptr;
			const std::string absoluteFilename = mapVirtualToAbsoluteFilename(fileMode, virtualFilename);
			if ( !absoluteFilename.empty() )
			{
				if ( FileMode::READ == fileMode )
				{
					file = new ::detail::AndroidReadFile(mAAssetManager, absoluteFilename);
				}
				else
				{
					// Error, not supported on Android
					RHI_ASSERT(false, "\"Renderer::IFileManager::openFile()\" with writing file mode isn't supported on Android")
						return nullptr;
				}
				if ( file->isInvalid() )
				{
					if ( mLog.print(Rhi::ILog::Type::CRITICAL, nullptr, __FILE__, static_cast<uint32_t>(__LINE__), "Failed to open file %s", virtualFilename) )
					{
						DEBUG_BREAK;
					}
					delete file;
					file = nullptr;
				}
				else
				{
#if SE_DEBUG
					++mNumberOfCurrentlyOpenedFiles;
					RHI_ASSERT(mNumberOfCurrentlyOpenedFiles < 256, "Too many simultaneously opened files")
#endif
				}
			}

			// Done
			return file;
		}

		inline virtual void closeFile(IFile& file) const override
		{
#if SE_DEBUG
			--mNumberOfCurrentlyOpenedFiles;
			RHI_ASSERT(mNumberOfCurrentlyOpenedFiles >= 0, "Error, more files closed as opened")
#endif
				delete static_cast<::detail::AndroidFile*>(&file);
		}

	protected:
		explicit AndroidFileManager(const AndroidFileManager&) = delete;
		AndroidFileManager& operator=(const AndroidFileManager&) = delete;

	private:
		typedef std::vector<std::string>								AbsoluteDirectoryNames;
		typedef std::unordered_map<std::string, AbsoluteDirectoryNames>	MountedDirectories;	// Key = UTF-8 mount point name (example: "MyProject"), value = absolute UTF-8 names of the mounted directories

	private:
		[[nodiscard]] inline bool getAbsoluteDirectoryNamesByMountPoint(VirtualFilename virtualFilename, const AbsoluteDirectoryNames** absoluteDirectoryNames, std::string& relativeFilename, std::string& mountPoint) const
		{
			RHI_ASSERT(nullptr != absoluteDirectoryNames, "Invalid absolute directory names")

				// Get mount point
				const std::string_view stdVirtualFilename = virtualFilename;
			const size_t slashIndex = stdVirtualFilename.find("/");
			if ( std::string::npos != slashIndex )
			{
				mountPoint = stdVirtualFilename.substr(0, slashIndex);
				MountedDirectories::const_iterator iterator = mMountedDirectories.find(mountPoint);
				if ( mMountedDirectories.cend() != iterator )
				{
					*absoluteDirectoryNames = &iterator->second;
					relativeFilename = stdVirtualFilename.substr(slashIndex + 1);

					// Done
					return true;
				}
				else
				{
					// Error!
					return false;
				}
			}
			else
			{
				// Use base directory
				*absoluteDirectoryNames = &mAbsoluteBaseDirectory;
				relativeFilename = virtualFilename;
				mountPoint.clear();

				// Done
				return true;
			}
		}

		[[nodiscard]] inline std::string mapVirtualToAbsoluteFilenameAndMountPoint(FileMode fileMode, VirtualFilename virtualFilename, std::string& mountPoint) const
		{
			// Sanity check
			RHI_ASSERT(nullptr != virtualFilename, "Invalid virtual filename")

				// Get absolute directory names
				const AbsoluteDirectoryNames* absoluteDirectoryNames = nullptr;
			std::string relativeFilename;
			if ( getAbsoluteDirectoryNamesByMountPoint(virtualFilename, &absoluteDirectoryNames, relativeFilename, mountPoint) && absoluteDirectoryNames != nullptr )
			{
				if ( mountPoint.empty() )
				{
					// Support for absolute filenames
					if ( ::detail::doesFileExist(mAAssetManager, virtualFilename) )
					{
						return virtualFilename;
					}
				}
				else
				{
					for ( const std::string& absoluteDirectoryName : *absoluteDirectoryNames )
					{
						const std::string absoluteFilename = FileSystemHelper::lexicallyNormal(absoluteDirectoryName + '/' + relativeFilename).generic_string();
						if ( ::detail::doesFileExist(mAAssetManager, absoluteFilename.c_str()) )
						{
							return absoluteFilename;
						}
					}

					// Still here and writing a file?
					if ( FileMode::WRITE == fileMode && !absoluteDirectoryNames->empty() )
					{
						return FileSystemHelper::lexicallyNormal((*absoluteDirectoryNames)[0] + '/' + relativeFilename).generic_string();
					}
				}
			}

			// Error!
			return "";
		}

	private:
		AAssetManager&		   mAAssetManager;			// Android asset manager
		Rhi::ILog&			   mLog;
		Rhi::IAssert&		   mAssert;
		DefaultAllocator&	   mAllocator;
		AbsoluteDirectoryNames mAbsoluteBaseDirectory;	// Absolute UTF-8 base directory, without "/" at the end
		MountedDirectories	   mMountedDirectories;
#if SE_DEBUG
		mutable int mNumberOfCurrentlyOpenedFiles = 0;	// For leak detection
#endif
	};
} // Renderer