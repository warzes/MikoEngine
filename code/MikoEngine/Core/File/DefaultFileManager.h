#pragma once

#include "Core/File/IFile.h"
#include "Core/File/IFileManager.h"
#include "Core/File/FileSystemHelper.h"

namespace
{
	namespace detail
	{
		static constexpr const char* DEFAULT_LOCAL_DATA_MOUNT_POINT = "LocalData";

		class DefaultFile : public IFile
		{
		public:
			inline DefaultFile()
			{
				// Nothing here
			}

			inline virtual ~DefaultFile() override
			{
				// Nothing here
			}

			[[nodiscard]] virtual bool isInvalid() const = 0;

		protected:
			explicit DefaultFile(const DefaultFile&) = delete;
			DefaultFile& operator=(const DefaultFile&) = delete;
		};

		class DefaultReadFile final : public DefaultFile
		{
		public:
			inline explicit DefaultReadFile(const std::string& absoluteFilename) :
				mFileStream(std_filesystem::u8path(absoluteFilename), std::ios::binary)
#if SE_DEBUG
				, mDebugName(absoluteFilename)
#endif
			{
				SE_ASSERT(mFileStream, "Failed to open default file for reading")
			}

			inline virtual ~DefaultReadFile() override
			{
				// Nothing here
			}

		public:
			[[nodiscard]] inline virtual bool isInvalid() const override
			{
				return !mFileStream;
			}

		public:
			[[nodiscard]] inline virtual size_t getNumberOfBytes() override
			{
				SE_ASSERT(mFileStream, "Invalid default file access")
					size_t numberOfBytes = 0;
				mFileStream.seekg(0, std::istream::end);
				numberOfBytes = static_cast<size_t>(mFileStream.tellg());
				mFileStream.seekg(0, std::istream::beg);
				return numberOfBytes;
			}

			inline virtual void read(void* destinationBuffer, size_t numberOfBytes) override
			{
				SE_ASSERT(nullptr != destinationBuffer, "Letting a file read into a null destination buffer is not allowed")
					SE_ASSERT(0 != numberOfBytes, "Letting a file read zero bytes is not allowed")
					SE_ASSERT(mFileStream, "Invalid default file access")
					mFileStream.read(reinterpret_cast<char*>(destinationBuffer), static_cast<std::streamsize>(numberOfBytes));
			}

			inline virtual void skip(size_t numberOfBytes) override
			{
				SE_ASSERT(0 != numberOfBytes, "Letting a file skip zero bytes is not allowed")
					SE_ASSERT(mFileStream, "Invalid default file access")
					mFileStream.ignore(static_cast<std::streamsize>(numberOfBytes));
			}

			inline virtual void write([[maybe_unused]] const void* sourceBuffer, [[maybe_unused]] size_t numberOfBytes) override
			{
				SE_ASSERT(nullptr != sourceBuffer, "Letting a file write from a null source buffer is not allowed")
					SE_ASSERT(0 != numberOfBytes, "Letting a file write zero bytes is not allowed")
					SE_ASSERT(mFileStream, "Invalid default file access")
					SE_ASSERT(false, "File write method not supported by the default implementation")
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
			explicit DefaultReadFile(const DefaultReadFile&) = delete;
			DefaultReadFile& operator=(const DefaultReadFile&) = delete;


			//[-------------------------------------------------------]
			//[ Private data                                          ]
			//[-------------------------------------------------------]
		private:
			std::ifstream mFileStream;
#if SE_DEBUG
			std::string mDebugName;	// Debug name for easier file identification when debugging
#endif


		};

		class DefaultWriteFile final : public DefaultFile
		{


			//[-------------------------------------------------------]
			//[ Public methods                                        ]
			//[-------------------------------------------------------]
		public:
			inline explicit DefaultWriteFile(const std::string& absoluteFilename) :
				mFileStream(std_filesystem::u8path(absoluteFilename), std::ios::binary)
#if SE_DEBUG
				, mDebugName(absoluteFilename)
#endif
			{
				SE_ASSERT(mFileStream, "Failed to open default file for writing")
			}

			inline virtual ~DefaultWriteFile() override
			{
				// Nothing here
			}


			//[-------------------------------------------------------]
			//[ Public virtual DefaultFile methods                    ]
			//[-------------------------------------------------------]
		public:
			[[nodiscard]] inline virtual bool isInvalid() const override
			{
				return !mFileStream;
			}


			//[-------------------------------------------------------]
			//[ Public virtual Renderer::IFile methods                ]
			//[-------------------------------------------------------]
		public:
			[[nodiscard]] inline virtual size_t getNumberOfBytes() override
			{
				SE_ASSERT(mFileStream, "Invalid default file access")
					SE_ASSERT(false, "File get number of bytes method not supported by the default implementation")
					return 0;
			}

			inline virtual void read([[maybe_unused]] void* destinationBuffer, [[maybe_unused]] size_t numberOfBytes) override
			{
				SE_ASSERT(nullptr != destinationBuffer, "Letting a file read into a null destination buffer is not allowed")
					SE_ASSERT(0 != numberOfBytes, "Letting a file read zero bytes is not allowed")
					SE_ASSERT(mFileStream, "Invalid default file access")
					SE_ASSERT(false, "File read method not supported by the default implementation")
			}

			inline virtual void skip([[maybe_unused]] size_t numberOfBytes) override
			{
				SE_ASSERT(0 != numberOfBytes, "Letting a file skip zero bytes is not allowed")
					SE_ASSERT(mFileStream, "Invalid default file access")
					SE_ASSERT(false, "File skip method not supported by the default implementation")
			}

			inline virtual void write(const void* sourceBuffer, size_t numberOfBytes) override
			{
				SE_ASSERT(nullptr != sourceBuffer, "Letting a file write from a null source buffer is not allowed")
					SE_ASSERT(0 != numberOfBytes, "Letting a file write zero bytes is not allowed")
					SE_ASSERT(mFileStream, "Invalid default file access")
					mFileStream.write(reinterpret_cast<const char*>(sourceBuffer), static_cast<std::streamsize>(numberOfBytes));
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
			explicit DefaultWriteFile(const DefaultWriteFile&) = delete;
			DefaultWriteFile& operator=(const DefaultWriteFile&) = delete;


			//[-------------------------------------------------------]
			//[ Private data                                          ]
			//[-------------------------------------------------------]
		private:
			std::ofstream mFileStream;
#if SE_DEBUG
			std::string mDebugName;	// Debug name for easier file identification when debugging
#endif


		};


		//[-------------------------------------------------------]
		//[ Anonymous detail namespace                            ]
		//[-------------------------------------------------------]
	} // detail
}

namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Default file manager implementation class one can use
	*
	*  @note
	*    - Designed to be instanced and used inside a single C++ file
	*    - Primarily for renderer toolkit with more relaxed write access
	*/
	class DefaultFileManager final : public IFileManager
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		inline DefaultFileManager(Rhi::ILog& log, Rhi::IAssert& assert, DefaultAllocator& allocator, const std::string& absoluteRootDirectory) :
			IFileManager(absoluteRootDirectory),
			mLog(log),
			mAssert(assert),
			mAllocator(allocator)
		{
			// Setup local data mount point
			mAbsoluteBaseDirectory.push_back(absoluteRootDirectory.c_str());
			createDirectories(::detail::DEFAULT_LOCAL_DATA_MOUNT_POINT);
			mountDirectory((absoluteRootDirectory + '/' + ::detail::DEFAULT_LOCAL_DATA_MOUNT_POINT).c_str(), ::detail::DEFAULT_LOCAL_DATA_MOUNT_POINT);
		}

		inline virtual ~DefaultFileManager() override
		{
			SE_ASSERT(0 == mNumberOfCurrentlyOpenedFiles, "File leak detected, not all opened files were closed")
		}


		//[-------------------------------------------------------]
		//[ Public virtual Renderer::IFileManager methods         ]
		//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline virtual const char* getLocalDataMountPoint() const override
		{
			return ::detail::DEFAULT_LOCAL_DATA_MOUNT_POINT;
		}

		[[nodiscard]] inline virtual const char* getMountPoint(const char* mountPoint) const override
		{
			SE_ASSERT(nullptr != mountPoint, "Invalid mount point")
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
			SE_ASSERT(nullptr != absoluteDirectoryName, "Invalid absolute directory name")
				SE_ASSERT(nullptr != mountPoint, "Invalid mount point")
#if SE_DEBUG
				// Additional sanity check: The same absolute directory name shouldn't be added to too different mount points
				for ( const auto& pair : mMountedDirectories )
				{
					if ( pair.first != mountPoint )
					{
						const AbsoluteDirectoryNames& absoluteDirectoryNames = pair.second;
						SE_ASSERT(absoluteDirectoryNames.cend() == std::find(absoluteDirectoryNames.begin(), absoluteDirectoryNames.end(), absoluteDirectoryName), "The same absolute directory name shouldn't be added to too different default mount points")
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
					SE_ASSERT(false, "Duplicate absolute default directory name detected, this situation should be avoided by the caller")
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
			std::string mountPoint;
			const std::string absoluteDirectoryName = mapVirtualToAbsoluteFilenameAndMountPoint(FileMode::READ, virtualDirectoryName, mountPoint);
			if ( !absoluteDirectoryName.empty() )
			{
				const size_t numberOfSkippedBytes = absoluteDirectoryName.length() + 1;	// +1 for '/'-slash at the end of the absolute directory name
				switch ( enumerationMode )
				{
				case EnumerationMode::ALL:
					for ( const std_filesystem::directory_entry& iterator : std_filesystem::recursive_directory_iterator(absoluteDirectoryName) )
					{
						virtualFilenames.push_back(mountPoint + '/' + iterator.path().generic_string().erase(0, numberOfSkippedBytes));
					}
					break;

				case EnumerationMode::FILES:
					for ( const std_filesystem::directory_entry& iterator : std_filesystem::recursive_directory_iterator(absoluteDirectoryName) )
					{
						if ( std_filesystem::is_regular_file(iterator) )
						{
							virtualFilenames.push_back(mountPoint + '/' + iterator.path().generic_string().erase(0, numberOfSkippedBytes));
						}
					}
					break;

				case EnumerationMode::DIRECTORIES:
					for ( const std_filesystem::directory_entry& iterator : std_filesystem::recursive_directory_iterator(absoluteDirectoryName) )
					{
						if ( std_filesystem::is_directory(iterator) )
						{
							virtualFilenames.push_back(mountPoint + '/' + iterator.path().generic_string().erase(0, numberOfSkippedBytes));
						}
					}
					break;
				}
			}
		}

		[[nodiscard]] inline virtual std::string mapVirtualToAbsoluteFilename(FileMode fileMode, VirtualFilename virtualFilename) const override
		{
			std::string mountPoint;
			return mapVirtualToAbsoluteFilenameAndMountPoint(fileMode, virtualFilename, mountPoint);
		}

		[[nodiscard]] inline virtual int64_t getLastModificationTime(VirtualFilename virtualFilename) const override
		{
			const std::string absoluteFilename = mapVirtualToAbsoluteFilename(FileMode::READ, virtualFilename);
			return absoluteFilename.empty() ? -1 : static_cast<int64_t>(std_filesystem::last_write_time(std_filesystem::u8path(absoluteFilename)).time_since_epoch().count());
		}

		[[nodiscard]] inline virtual int64_t getFileSize(VirtualFilename virtualFilename) const override
		{
			const std::string absoluteFilename = mapVirtualToAbsoluteFilename(FileMode::READ, virtualFilename);
			return absoluteFilename.empty() ? -1 : static_cast<int64_t>(std_filesystem::file_size(std_filesystem::u8path(absoluteFilename)));
		}

		inline virtual bool createDirectories(VirtualDirectoryName virtualDirectoryName) const override
		{
			// Sanity check
			SE_ASSERT(nullptr != virtualDirectoryName, "Invalid virtual directory name")

				// Create directories
				const AbsoluteDirectoryNames* absoluteDirectoryNames = nullptr;
			std::string relativeFilename;
			std::string mountPoint;
			if ( getAbsoluteDirectoryNamesByMountPoint(virtualDirectoryName, &absoluteDirectoryNames, relativeFilename, mountPoint) && absoluteDirectoryNames != nullptr && !absoluteDirectoryNames->empty() )
			{
				// Do only care about the first hit mount point
				const std_filesystem::path absoluteFilename = std_filesystem::u8path(absoluteDirectoryNames->at(0) + '/' + relativeFilename);
				if ( !std_filesystem::exists(absoluteFilename) && !std_filesystem::create_directories(absoluteFilename) )
				{
					// Failed to create the directories
					return false;
				}
			}

			// Directories have been created successfully
			return true;
		}

		[[nodiscard]] inline virtual IFile* openFile(FileMode fileMode, VirtualFilename virtualFilename) const override
		{
			::detail::DefaultFile* file = nullptr;
			const std::string absoluteFilename = mapVirtualToAbsoluteFilename(fileMode, virtualFilename);
			if ( !absoluteFilename.empty() )
			{
				if ( FileMode::READ == fileMode )
				{
					file = new ::detail::DefaultReadFile(absoluteFilename);
				}
				else
				{
					file = new ::detail::DefaultWriteFile(absoluteFilename);
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
					SE_ASSERT(mNumberOfCurrentlyOpenedFiles < 256, "Too many simultaneously opened files. The default limit on Microsoft Windows is 512 (can be changed via _setmaxstdio()) and on Mac OS X 256.")
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
			SE_ASSERT(mNumberOfCurrentlyOpenedFiles >= 0, "Error, more files closed as opened")
#endif
				delete static_cast<::detail::DefaultFile*>(&file);
		}


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
	protected:
		explicit DefaultFileManager(const DefaultFileManager&) = delete;
		DefaultFileManager& operator=(const DefaultFileManager&) = delete;


		//[-------------------------------------------------------]
		//[ Private definitions                                   ]
		//[-------------------------------------------------------]
	private:
		typedef std::vector<std::string>								AbsoluteDirectoryNames;
		typedef std::unordered_map<std::string, AbsoluteDirectoryNames>	MountedDirectories;	// Key = UTF-8 mount point name (example: "MyProject"), value = absolute UTF-8 names of the mounted directories (example: "c:/MyProject")


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		[[nodiscard]] inline bool getAbsoluteDirectoryNamesByMountPoint(VirtualFilename virtualFilename, const AbsoluteDirectoryNames** absoluteDirectoryNames, std::string& relativeFilename, std::string& mountPoint) const
		{
			SE_ASSERT(nullptr != absoluteDirectoryNames, "Invalid absolute directory names")

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
			SE_ASSERT(nullptr != virtualFilename, "Invalid virtual filename")

				// Get absolute directory names
				const AbsoluteDirectoryNames* absoluteDirectoryNames = nullptr;
			std::string relativeFilename;
			if ( getAbsoluteDirectoryNamesByMountPoint(virtualFilename, &absoluteDirectoryNames, relativeFilename, mountPoint) && absoluteDirectoryNames != nullptr )
			{
				if ( mountPoint.empty() )
				{
					// Support for absolute filenames
					if ( std_filesystem::exists(std_filesystem::u8path(virtualFilename)) )
					{
						return virtualFilename;
					}
				}
				else
				{
					for ( const std::string& absoluteDirectoryName : *absoluteDirectoryNames )
					{
						const std::string absoluteFilename = FileSystemHelper::lexicallyNormal(absoluteDirectoryName + '/' + relativeFilename).generic_string();
						if ( std_filesystem::exists(std_filesystem::u8path(absoluteFilename)) )
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


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		Rhi::IAssert&		   mAssert;
		DefaultAllocator&	   mAllocator;
		AbsoluteDirectoryNames mAbsoluteBaseDirectory;	// Absolute UTF-8 base directory, without "/" at the end
		MountedDirectories	   mMountedDirectories;
#if SE_DEBUG
		mutable int mNumberOfCurrentlyOpenedFiles = 0;	// For leak detection
#endif


	};


	//[-------------------------------------------------------]
	//[ Namespace                                             ]
	//[-------------------------------------------------------]
} // Renderer
