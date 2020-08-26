#pragma once

#include "Core/File/IFile.h"
#include "Core/File/IFileManager.h"
#include "Core/File/FileSystemHelper.h"

// Disable warnings in external headers, we can't fix them
SE_PRAGMA_WARNING_PUSH
SE_PRAGMA_WARNING_DISABLE_MSVC(4668)	// warning C4668: '__GNUC__' is not defined as a preprocessor macro, replacing with '0' for '#if/#elif'
extern "C"
{
#include <PhysicsFS/physfs.h>
}
SE_PRAGMA_WARNING_POP

namespace
{
	namespace detail
	{
		static constexpr const char* PHYSICSFS_LOCAL_DATA_MOUNT_POINT = "LocalData";

		void writePhysicsFSErrorToLog()
		{
			const char* errorAsString = PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode());
			if (nullptr != errorAsString)
			{
				//if (log.print(Rhi::ILog::Type::CRITICAL, nullptr, __FILE__, static_cast<uint32_t>(__LINE__), "PhysicsFS error: %s", errorAsString))
				{
					SE_DEBUG_BREAK;
				}
			}
			else
			{
				SE_ASSERT(false, "Failed to map PhysicsFS error code to error as string")
			}
		}

		class PhysicsFSFile : public IFile
		{
		public:
			inline PhysicsFSFile()
			{
				// Nothing here
			}

			inline virtual ~PhysicsFSFile() override
			{
				// Nothing here
			}

		public:
			[[nodiscard]] virtual bool isInvalid() const = 0;

		protected:
			explicit PhysicsFSFile(const PhysicsFSFile&) = delete;
			PhysicsFSFile& operator=(const PhysicsFSFile&) = delete;
		};

		class PhysicsFSReadFile final : public PhysicsFSFile
		{
		public:
			inline explicit PhysicsFSReadFile(const char* virtualFilename) :
				mPhysicsFSFile(PHYSFS_openRead(virtualFilename))
#if SE_DEBUG
				, mDebugName(virtualFilename)
#endif
			{
				SE_ASSERT(nullptr != mPhysicsFSFile, "Failed to open PhysicsFS file for reading")
			}

			inline virtual ~PhysicsFSReadFile() override
			{
				if (nullptr != mPhysicsFSFile)
				{
					[[maybe_unused]] const int result = PHYSFS_close(mPhysicsFSFile);
					SE_ASSERT(0 != result, "Failed to close read PhysicsFS file")
				}
			}

		public:
			[[nodiscard]] inline virtual bool isInvalid() const override
			{
				return (nullptr == mPhysicsFSFile);
			}

		public:
			[[nodiscard]] inline virtual size_t getNumberOfBytes() override
			{
				SE_ASSERT(nullptr != mPhysicsFSFile, "Invalid PhysicsFS file access")
					const PHYSFS_sint64 fileLength = PHYSFS_fileLength(mPhysicsFSFile);
				SE_ASSERT(-1 != fileLength, "PhysicsFS failed to determine the file size")
					return static_cast<size_t>(fileLength);
			}

			inline virtual void read(void* destinationBuffer, size_t numberOfBytes) override
			{
				SE_ASSERT(nullptr != destinationBuffer, "Letting a file read into a null destination buffer is not allowed")
					SE_ASSERT(0 != numberOfBytes, "Letting a file read zero bytes is not allowed")
					SE_ASSERT(nullptr != mPhysicsFSFile, "Invalid PhysicsFS file access")
					[[maybe_unused]] const PHYSFS_sint64 numberOfReadBytes = PHYSFS_readBytes(mPhysicsFSFile, destinationBuffer, numberOfBytes);
				SE_ASSERT(static_cast<size_t>(numberOfReadBytes) == numberOfBytes, "PhysicsFS failed to read all requested bytes")	// We're restrictive by intent
			}

			inline virtual void skip(size_t numberOfBytes) override
			{
				SE_ASSERT(0 != numberOfBytes, "Letting a file skip zero bytes is not allowed")
					SE_ASSERT(nullptr != mPhysicsFSFile, "Invalid PhysicsFS file access")
					const PHYSFS_sint64 currentOffset = PHYSFS_tell(mPhysicsFSFile);
				SE_ASSERT(-1 != currentOffset, "PhysicsFS failed to retrieve the current file offset")
					[[maybe_unused]] const int result = PHYSFS_seek(mPhysicsFSFile, static_cast<PHYSFS_uint64>(currentOffset + numberOfBytes));
				SE_ASSERT(0 != result, "PhysicsFS failed seek file")
			}

			inline virtual void write([[maybe_unused]] const void* sourceBuffer, [[maybe_unused]] size_t numberOfBytes) override
			{
				SE_ASSERT(nullptr != sourceBuffer, "Letting a file write from a null source buffer is not allowed")
					SE_ASSERT(0 != numberOfBytes, "Letting a file write zero bytes is not allowed")
					SE_ASSERT(nullptr != mPhysicsFSFile, "Invalid PhysicsFS file access")
					SE_ASSERT(false, "File write method not supported by the PhysicsFS implementation")
			}

#if SE_DEBUG
			[[nodiscard]] inline virtual const char* getDebugFilename() const override
			{
				return mDebugName.c_str();
			}
#endif

		protected:
			explicit PhysicsFSReadFile(const PhysicsFSReadFile&) = delete;
			PhysicsFSReadFile& operator=(const PhysicsFSReadFile&) = delete;

		private:
			PHYSFS_File* mPhysicsFSFile;
#if SE_DEBUG
			std::string mDebugName;	// Debug name for easier file identification when debugging
#endif
		};

		class PhysicsFSWriteFile final : public PhysicsFSFile
		{
		public:
			inline explicit PhysicsFSWriteFile(const char* virtualFilename) :
				mPhysicsFSFile(PHYSFS_openWrite(virtualFilename))
#if SE_DEBUG
				, mDebugName(virtualFilename)
#endif
			{
				SE_ASSERT(nullptr != mPhysicsFSFile, "Failed to open PhysicsFS file for writing")
			}

			inline virtual ~PhysicsFSWriteFile() override
			{
				if (nullptr != mPhysicsFSFile)
				{
					[[maybe_unused]] const int result = PHYSFS_close(mPhysicsFSFile);
					SE_ASSERT(0 != result, "Failed to close written PhysicsFS file")
				}
			}

		public:
			[[nodiscard]] inline virtual bool isInvalid() const override
			{
				return (nullptr == mPhysicsFSFile);
			}


			//[-------------------------------------------------------]
			//[ Public virtual Renderer::IFile methods                ]
			//[-------------------------------------------------------]
		public:
			[[nodiscard]] inline virtual size_t getNumberOfBytes() override
			{
				SE_ASSERT(nullptr != mPhysicsFSFile, "Invalid PhysicsFS file access")
					SE_ASSERT(false, "File get number of bytes method not supported by the PhysicsFS implementation")
					return 0;
			}

			inline virtual void read(void*, size_t) override
			{
				SE_ASSERT(nullptr != mPhysicsFSFile, "Invalid PhysicsFS file access")
					SE_ASSERT(false, "File read method not supported by the PhysicsFS implementation")
			}

			inline virtual void skip(size_t) override
			{
				SE_ASSERT(nullptr != mPhysicsFSFile, "Invalid PhysicsFS file access")
					SE_ASSERT(false, "File skip method not supported by the PhysicsFS implementation")
			}

			inline virtual void write(const void* sourceBuffer, size_t numberOfBytes) override
			{
				SE_ASSERT(nullptr != mPhysicsFSFile, "Invalid PhysicsFS file access")
					[[maybe_unused]] const PHYSFS_sint64 numberOfWrittenBytes = PHYSFS_writeBytes(mPhysicsFSFile, sourceBuffer, numberOfBytes);
				SE_ASSERT(static_cast<size_t>(numberOfWrittenBytes) == numberOfBytes, "PhysicsFS failed to write all requested bytes")	// We're restrictive by intent
			}

#if SE_DEBUG
			[[nodiscard]] inline virtual const char* getDebugFilename() const override
			{
				return mDebugName.c_str();
			}
#endif


			//[-------------------------------------------------------]
			//[ Protected methods                                     ]
			//[-------------------------------------------------------]
		protected:
			explicit PhysicsFSWriteFile(const PhysicsFSWriteFile&) = delete;
			PhysicsFSWriteFile& operator=(const PhysicsFSWriteFile&) = delete;


			//[-------------------------------------------------------]
			//[ Private data                                          ]
			//[-------------------------------------------------------]
		private:
			PHYSFS_File* mPhysicsFSFile;
#if SE_DEBUG
			std::string mDebugName;	// Debug name for easier file identification when debugging
#endif
		};
	} // detail
}

/**
*  @brief
*    PhysicsFS ( https://icculus.org/physfs/ ) file manager implementation class one can use
*
*  @note
*    - Designed to be instanced and used inside a single C++ file
*    - Primarily for renderer with restricted write access
*/
class PhysicsFSFileManager final : public IFileManager
{
public:
	inline PhysicsFSFileManager(const std::string& absoluteRootDirectory, bool ownsPhysicsFSInstance = true) :
		IFileManager(absoluteRootDirectory),
		mOwnsPhysicsFSInstance(ownsPhysicsFSInstance)
	{
		// Initialize the PhysicsFS library
		if (!mOwnsPhysicsFSInstance || 0 != PHYSFS_init(nullptr))
		{
			if (mOwnsPhysicsFSInstance && 0 == PHYSFS_setWriteDir(absoluteRootDirectory.c_str()))
			{
				// Error!
				::detail::writePhysicsFSErrorToLog();
			}

			// Setup local data mount point
			createDirectories(::detail::PHYSICSFS_LOCAL_DATA_MOUNT_POINT);
			mountDirectory((absoluteRootDirectory + '/' + ::detail::PHYSICSFS_LOCAL_DATA_MOUNT_POINT).c_str(), ::detail::PHYSICSFS_LOCAL_DATA_MOUNT_POINT);
		}
		else
		{
			// Error!
			::detail::writePhysicsFSErrorToLog();
		}
	}

	inline virtual ~PhysicsFSFileManager() override
	{
		SE_ASSERT(0 == mNumberOfCurrentlyOpenedFiles, "File leak detected, not all opened files were closed")

			// Deinitialize the PhysicsFS library
			if (mOwnsPhysicsFSInstance && 0 == PHYSFS_deinit())
			{
				// Error!
				::detail::writePhysicsFSErrorToLog();
			}
	}


	//[-------------------------------------------------------]
	//[ Public virtual Renderer::IFileManager methods         ]
	//[-------------------------------------------------------]
public:
	[[nodiscard]] inline virtual const char* getLocalDataMountPoint() const override
	{
		return ::detail::PHYSICSFS_LOCAL_DATA_MOUNT_POINT;
	}

	[[nodiscard]] inline virtual const char* getMountPoint(const char* mountPoint) const override
	{
		SE_ASSERT(nullptr != mountPoint, "Invalid mount point")
			return PHYSFS_getMountPoint(mountPoint);
	}

	inline virtual bool mountDirectory(AbsoluteDirectoryName absoluteDirectoryName, const char* mountPoint, bool appendToPath = false) override
	{
		// Sanity check
		SE_ASSERT(nullptr != absoluteDirectoryName, "Invalid absolute directory name")
			SE_ASSERT(nullptr != mountPoint, "Invalid mount point")

			// Mount directory
			if (0 == PHYSFS_mount(absoluteDirectoryName, mountPoint, appendToPath))
			{
				// Error!
				::detail::writePhysicsFSErrorToLog();
				return false;
			}

		// Done
		return true;
	}

	[[nodiscard]] inline virtual bool doesFileExist(VirtualFilename virtualFilename) const override
	{
		// Sanity check
		SE_ASSERT(nullptr != virtualFilename, "Invalid virtual filename")

			// Check for file existence
			return (0 != PHYSFS_exists(virtualFilename));
	}

	inline virtual void enumerateFiles(VirtualDirectoryName virtualDirectoryName, EnumerationMode enumerationMode, std::vector<std::string>& virtualFilenames) const override
	{
		char** physicsFsFilenames = PHYSFS_enumerateFiles(virtualDirectoryName);
		switch (enumerationMode)
		{
		case EnumerationMode::ALL:
		{
			for (char** physicsFsFilename = physicsFsFilenames; nullptr != *physicsFsFilename; ++physicsFsFilename)
			{
				virtualFilenames.push_back(*physicsFsFilename);
			}
			break;
		}

		case EnumerationMode::FILES:
		{
			const std::string stdVirtualDirectoryName = virtualDirectoryName;
			for (char** physicsFsFilename = physicsFsFilenames; nullptr != *physicsFsFilename; ++physicsFsFilename)
			{
				if (PHYSFS_isDirectory((stdVirtualDirectoryName + '/' + *physicsFsFilename).c_str()) == 0)
				{
					virtualFilenames.push_back(*physicsFsFilename);
				}
			}
			break;
		}

		case EnumerationMode::DIRECTORIES:
		{
			const std::string stdVirtualDirectoryName = virtualDirectoryName;
			for (char** physicsFsFilename = physicsFsFilenames; nullptr != *physicsFsFilename; ++physicsFsFilename)
			{
				if (PHYSFS_isDirectory((stdVirtualDirectoryName + '/' + *physicsFsFilename).c_str()) != 0)
				{
					virtualFilenames.push_back(*physicsFsFilename);
				}
			}
			break;
		}
		}
		PHYSFS_freeList(physicsFsFilenames);
	}

	[[nodiscard]] inline virtual std::string mapVirtualToAbsoluteFilename(FileMode fileMode, VirtualFilename virtualFilename) const override
	{
		// Figure out where in the search path a file resides (e.g. "LocalData/EngineImGuiLayout.ini" -> "c:/MyProject/Binary/LocalData")
		const char* realDirectory = PHYSFS_getRealDir(virtualFilename);
		if (nullptr != realDirectory)
		{
			// Determine a mounted archive mount point (e.g. "c:/MyProject/Binary/LocalData" -> "LocalData")
			const char* mountPoint = PHYSFS_getMountPoint(realDirectory);
			if (nullptr == mountPoint)
			{
				// The mount point is the root, so, determining the absolute filename is trivial
				return std::string(realDirectory) + '/' + virtualFilename;
			}
			else
			{
				// Find the mount point part inside the given virtual filename
				const size_t index = std::string_view(virtualFilename).find_first_of(mountPoint);
				if (std::string::npos != index)
				{
					// Now that we have all information we need, transform the given virtual filename into an absolute filename
					// -> Example: The virtual filename "LocalData/EngineImGuiLayout.ini" will result in the absolute filename "c:/MyProject/Binary/LocalData/EngineImGuiLayout.ini"
					std::string absoluteFilename = virtualFilename;
					absoluteFilename.erase(index, strlen(mountPoint));	// Example: "LocalData/EngineImGuiLayout.ini" to "EngineImGuiLayout.ini"
					absoluteFilename = FileSystemHelper::lexicallyNormal(std::string(realDirectory) + '/' + absoluteFilename).generic_string();

					// Done
					return absoluteFilename;
				}
			}
		}

		// File not found, guess location of a newly created file?
		else if (FileMode::WRITE == fileMode)
		{
			// Get the absolute filename of the directory a newly created file would be in
			// -> Example: The virtual filename "LocalData/EngineImGuiLayout.ini" will result in the absolute directory name "c:/MyProject/Binary/LocalData"
			const std_filesystem::path path(virtualFilename);
			const std::string absoluteDirectoryName = mapVirtualToAbsoluteFilename(fileMode, path.parent_path().generic_string().c_str());
			if (!absoluteDirectoryName.empty())
			{
				// Construct the absolute filename
				return absoluteDirectoryName + '/' + path.filename().string();
			}
		}

		// Error!
		SE_ASSERT(false, "Failed to map virtual to PhysicsFS absolute filename")
			return "";
	}

	[[nodiscard]] inline virtual int64_t getLastModificationTime(VirtualFilename virtualFilename) const override
	{
		// Sanity check
		SE_ASSERT(nullptr != virtualFilename, "Invalid virtual filename")

			// Ask PhysicsFS
			PHYSFS_Stat physicsFSStat = {};
		if (0 == PHYSFS_stat(virtualFilename, &physicsFSStat))
		{
			// Error!
			SE_ASSERT(false, "Failed to get PhysicsFS last file modification time")
				::detail::writePhysicsFSErrorToLog();
			return 0;
		}
		else
		{
			return physicsFSStat.modtime;
		}
	}

	[[nodiscard]] inline virtual int64_t getFileSize(VirtualFilename virtualFilename) const override
	{
		// Sanity check
		SE_ASSERT(nullptr != virtualFilename, "Invalid virtual filename")

			// Ask PhysicsFS
			PHYSFS_Stat physicsFSStat = {};
		if (0 == PHYSFS_stat(virtualFilename, &physicsFSStat))
		{
			// Error!
			SE_ASSERT(false, "Failed to get PhysicsFS file size")
				::detail::writePhysicsFSErrorToLog();
			return 0;
		}
		else
		{
			return physicsFSStat.filesize;
		}
	}

	inline virtual bool createDirectories(VirtualDirectoryName virtualDirectoryName) const override
	{
		// Sanity check
		SE_ASSERT(nullptr != virtualDirectoryName, "Invalid virtual directory name")

			// Create directories
			const int result = PHYSFS_mkdir(virtualDirectoryName);
		SE_ASSERT(0 != result, "PhysicsFS failed to create the directories")
			return (result != 0);
	}

	[[nodiscard]] inline virtual IFile* openFile(FileMode fileMode, VirtualFilename virtualFilename) const override
	{
		// Sanity check
		SE_ASSERT(nullptr != virtualFilename, "Invalid virtual filename")

			// Open file
			::detail::PhysicsFSFile* file = nullptr;
		if (FileMode::READ == fileMode)
		{
			file = new ::detail::PhysicsFSReadFile(virtualFilename);
		}
		else
		{
			file = new ::detail::PhysicsFSWriteFile(virtualFilename);
		}
		if (file->isInvalid())
		{
			//if (mLog.print(Rhi::ILog::Type::CRITICAL, nullptr, __FILE__, static_cast<uint32_t>(__LINE__), "Failed to open file %s", virtualFilename))
			{
				SE_DEBUG_BREAK;
			}
			delete file;
			file = nullptr;
		}
		else
		{
#if SE_DEBUG
			++mNumberOfCurrentlyOpenedFiles;
			SE_ASSERT(mNumberOfCurrentlyOpenedFiles < 256, "Too many simultaneously opened files. The default limit on Microsoft Windows is 512 (can be changed via _setmaxstdio()) and on Mac OS X 256.")
#endif
		}
		return file;
	}

	inline virtual void closeFile(IFile& file) const override
	{
#if SE_DEBUG
		--mNumberOfCurrentlyOpenedFiles;
		SE_ASSERT(mNumberOfCurrentlyOpenedFiles >= 0, "Error, more files closed as opened")
#endif
			delete static_cast<::detail::PhysicsFSFile*>(&file);
	}

protected:
	explicit PhysicsFSFileManager(const PhysicsFSFileManager&) = delete;
	PhysicsFSFileManager& operator=(const PhysicsFSFileManager&) = delete;

private:
	bool	   mOwnsPhysicsFSInstance;
#if SE_DEBUG
	mutable int mNumberOfCurrentlyOpenedFiles = 0;	// For leak detection
#endif
};