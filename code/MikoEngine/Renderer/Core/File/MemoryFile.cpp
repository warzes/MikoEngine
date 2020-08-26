#include "stdafx.h"
#include "Renderer/Core/File/MemoryFile.h"
#include "Renderer/Core/File/IFileManager.h"

// Disable warnings in external headers, we can't fix them
SE_PRAGMA_WARNING_PUSH
	SE_PRAGMA_WARNING_DISABLE_MSVC(4668)	// warning C4668: '__GNUC__' is not defined as a preprocessor macro, replacing with '0' for '#if/#elif'
	#include <lz4/lz4hc.h>
SE_PRAGMA_WARNING_POP


//[-------------------------------------------------------]
//[ Anonymous detail namespace                            ]
//[-------------------------------------------------------]
namespace
{
	namespace detail
	{


		//[-------------------------------------------------------]
		//[ Global definitions                                    ]
		//[-------------------------------------------------------]
		struct FileFormatHeader final
		{
			// Format
			uint32_t formatType;
			uint32_t formatVersion;
			// Content
			uint32_t numberOfCompressedBytes;
			uint32_t numberOfDecompressedBytes;
		};


//[-------------------------------------------------------]
//[ Anonymous detail namespace                            ]
//[-------------------------------------------------------]
	} // detail
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	bool MemoryFile::loadLz4CompressedDataByVirtualFilename(uint32_t formatType, uint32_t formatVersion, const IFileManager& fileManager, VirtualFilename virtualFilename)
	{
		bool result = false;
		IFile* file = fileManager.openFile(IFileManager::FileMode::READ, virtualFilename);
		if (nullptr != file)
		{
			// Tell the memory mapped file about the LZ4 compressed data
			result = loadLz4CompressedDataFromFile(formatType, formatVersion, *file);
			#if SE_DEBUG
				mDebugName = virtualFilename;
			#endif

			// Close file
			fileManager.closeFile(*file);
		}

		// Done
		return result;
	}

	bool MemoryFile::loadLz4CompressedDataFromFile(uint32_t formatType, uint32_t formatVersion, IFile& file)
	{
		// Read in the file format header
		::detail::FileFormatHeader fileFormatHeader;
		file.read(&fileFormatHeader, sizeof(::detail::FileFormatHeader));
		if (formatType == fileFormatHeader.formatType && formatVersion == fileFormatHeader.formatVersion)
		{
			// Tell the memory mapped file about the LZ4 compressed data
			setLz4CompressedDataByFile(file, fileFormatHeader.numberOfCompressedBytes, fileFormatHeader.numberOfDecompressedBytes);
			#if SE_DEBUG
				mDebugName = file.getDebugFilename();
			#endif

			// Done
			return true;
		}
		else
		{
			// Error!
			SE_ASSERT(false, "Invalid file format header")
			return false;
		}
	}

	void MemoryFile::setLz4CompressedDataByFile(IFile& file, uint32_t numberOfCompressedBytes, uint32_t numberOfDecompressedBytes)
	{
		// Sanity checks
		SE_ASSERT(0 != numberOfCompressedBytes, "Zero LZ4 compressed bytes are invalid")
		SE_ASSERT(0 != numberOfDecompressedBytes, "Zero LZ4 decompressed bytes are invalid")

		// Read data
		mNumberOfDecompressedBytes = numberOfDecompressedBytes;
		mDecompressedData.clear();
		mCurrentDataPointer = nullptr;
		mCompressedData.resize(numberOfCompressedBytes);
		file.read(mCompressedData.data(), numberOfCompressedBytes);
	}

	void MemoryFile::decompress()
	{
		mDecompressedData.resize(mNumberOfDecompressedBytes);
		[[maybe_unused]] const int numberOfDecompressedBytes = LZ4_decompress_safe(reinterpret_cast<const char*>(mCompressedData.data()), reinterpret_cast<char*>(mDecompressedData.data()), static_cast<int>(mCompressedData.size()), static_cast<int>(mNumberOfDecompressedBytes));
		SE_ASSERT(mNumberOfDecompressedBytes == static_cast<uint32_t>(numberOfDecompressedBytes), "Invalid number of decompressed bytes")
		mCurrentDataPointer = mDecompressedData.data();
	}

	bool MemoryFile::writeLz4CompressedDataByVirtualFilename(uint32_t formatType, uint32_t formatVersion, const IFileManager& fileManager, VirtualFilename virtualFilename) const
	{
		// Open file
		IFile* file = fileManager.openFile(IFileManager::FileMode::WRITE, virtualFilename);
		if (nullptr != file)
		{
			// Write file
			const int destinationCapacity = LZ4_compressBound(static_cast<int>(mDecompressedData.size()));
			char* destination = new char[static_cast<unsigned int>(destinationCapacity)];
			{
				const int numberOfWrittenBytes = LZ4_compress_HC(reinterpret_cast<const char*>(mDecompressedData.data()), destination, static_cast<int>(mDecompressedData.size()), destinationCapacity, LZ4HC_CLEVEL_MAX);

				{ // Write down the file format header
					::detail::FileFormatHeader fileFormatHeader;
					fileFormatHeader.formatType				   = formatType;
					fileFormatHeader.formatVersion			   = formatVersion;
					fileFormatHeader.numberOfCompressedBytes   = static_cast<uint32_t>(numberOfWrittenBytes);
					fileFormatHeader.numberOfDecompressedBytes = static_cast<uint32_t>(mDecompressedData.size());
					file->write(&fileFormatHeader, sizeof(::detail::FileFormatHeader));
				}

				// Write down the compressed data
				file->write(destination, static_cast<size_t>(numberOfWrittenBytes));
			}
			delete [] destination;

			// Close file
			fileManager.closeFile(*file);

			// Done
			return true;
		}

		// Error!
		return false;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer
