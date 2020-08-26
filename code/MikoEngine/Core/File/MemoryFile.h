#pragma once

#include "Core/File/IFile.h"
#include "RHI/Rhi.h"

class IFileManager;

typedef const char* VirtualFilename;	// UTF-8 virtual filename, the virtual filename scheme is "<mount point = project name>/<asset directory>/<asset name>.<file extension>" (example "Example/Mesh/Monster/Squirrel.mesh"), never ever a null pointer and always finished by a terminating zero


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Memory mapped file
*
*  @note
*    - Supports LZ4 compression ( http://lz4.github.io/lz4/ )
*    - Designed for instance re-usage
*/
class MemoryFile final : public IFile
{
public:
	typedef std::vector<uint8_t> ByteVector;

public:
	inline MemoryFile() :
		mNumberOfDecompressedBytes(0),
		mCurrentDataPointer(nullptr)
	{
		// Nothing here
	}

	inline MemoryFile(size_t reserveNumberOfCompressedBytes, size_t reserveNumberOfDecompressedBytes) :
		mNumberOfDecompressedBytes(0),
		mCurrentDataPointer(nullptr)
	{
		mCompressedData.reserve(reserveNumberOfCompressedBytes);
		mDecompressedData.reserve(reserveNumberOfDecompressedBytes);
	}

	inline virtual ~MemoryFile() override
	{
		// Nothing here
	}

	[[nodiscard]] inline ByteVector& getByteVector()
	{
		return mDecompressedData;
	}

	[[nodiscard]] inline const ByteVector& getByteVector() const
	{
		return mDecompressedData;
	}

	[[nodiscard]] bool loadLz4CompressedDataByVirtualFilename(uint32_t formatType, uint32_t formatVersion, const IFileManager& fileManager, VirtualFilename virtualFilename);
	[[nodiscard]] bool loadLz4CompressedDataFromFile(uint32_t formatType, uint32_t formatVersion, IFile& file);
	void setLz4CompressedDataByFile(IFile& file, uint32_t numberOfCompressedBytes, uint32_t numberOfDecompressedBytes);
	void decompress();
	[[nodiscard]] bool writeLz4CompressedDataByVirtualFilename(uint32_t formatType, uint32_t formatVersion, const IFileManager& fileManager, VirtualFilename virtualFilename) const;

public:
	[[nodiscard]] inline virtual size_t getNumberOfBytes() override
	{
		return mDecompressedData.size();
	}

	inline virtual void read(void* destinationBuffer, size_t numberOfBytes) override
	{
		SE_ASSERT(nullptr != destinationBuffer, "Letting a file read into a null destination buffer is not allowed")
			SE_ASSERT(0 != numberOfBytes, "Letting a file read zero bytes is not allowed")
			SE_ASSERT((mCurrentDataPointer - mDecompressedData.data()) + numberOfBytes <= mDecompressedData.size(), "Invalid number of bytes")
			memcpy(destinationBuffer, mCurrentDataPointer, numberOfBytes);
		mCurrentDataPointer += numberOfBytes;
	}

	inline virtual void skip(size_t numberOfBytes) override
	{
		SE_ASSERT(0 != numberOfBytes, "Letting a file skip zero bytes is not allowed")
			SE_ASSERT((mCurrentDataPointer - mDecompressedData.data()) + numberOfBytes <= mDecompressedData.size(), "Invalid number of bytes")
			mCurrentDataPointer += numberOfBytes;
	}

	inline virtual void write(const void* sourceBuffer, size_t numberOfBytes) override
	{
		SE_ASSERT(nullptr != sourceBuffer, "Letting a file write from a null source buffer is not allowed")
			SE_ASSERT(0 != numberOfBytes, "Letting a file write zero bytes is not allowed")
			std::copy(static_cast<const uint8_t*>(sourceBuffer), static_cast<const uint8_t*>(sourceBuffer) + numberOfBytes, std::back_inserter(mDecompressedData));
	}

#if SE_DEBUG
	[[nodiscard]] inline virtual const char* getDebugFilename() const override
	{
		return mDebugName.c_str();
	}
#endif

protected:
	explicit MemoryFile(const MemoryFile&) = delete;
	MemoryFile& operator=(const MemoryFile&) = delete;

private:
	ByteVector mCompressedData;		// Owns the data
	ByteVector mDecompressedData;	// Owns the data
	uint32_t   mNumberOfDecompressedBytes;
	uint8_t* mCurrentDataPointer;	// Pointer to the current uncompressed data position, doesn't own the data
#if SE_DEBUG
	std::string mDebugName;	// Debug name for easier file identification when debugging
#endif
};