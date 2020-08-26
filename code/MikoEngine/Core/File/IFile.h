#pragma once

/**
*  @brief
*    Abstract file interface
*
*  @note
*    - There's no "seek()" or "tell()" by intent, the renderer toolkit can transform the data so during runtime the most simple file access pattern is sufficient
*    - It's only called "file interface" because this is usually the name for such a concept, doesn't mean the concrete implementation needs to work with files
*    - Renderer loaders try to read bigger chunks as often as possible instead of too fine granular byte wise read calls
*/
class IFile
{
public:
	//[-------------------------------------------------------]
	//[ Read                                                  ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Return the number of bytes inside the file
	*
	*  @return
	*    The number of bytes inside the file
	*/
	[[nodiscard]] virtual size_t getNumberOfBytes() = 0;

	/**
	*  @brief
	*    Read a requested number of bytes from the file
	*
	*  @param[out] destinationBuffer
	*    Destination buffer were to write to, must be at least "numberOfBytes" long, never ever a null pointer
	*  @param[in] numberOfBytes
	*    Number of bytes to write into the destination buffer, it's the callers responsibility that this number of byte is correct
	*/
	virtual void read(void* destinationBuffer, size_t numberOfBytes) = 0;

	/**
	*  @brief
	*    Skip a requested number of bytes
	*
	*  @param[in] numberOfBytes
	*    Number of bytes to skip, it's the callers responsibility that this number of byte is correct
	*/
	virtual void skip(size_t numberOfBytes) = 0;

	//[-------------------------------------------------------]
	//[ Write                                                 ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Write a requested number of bytes into the file
	*
	*  @param[in] sourceBuffer
	*    Source buffer were to read from, must be at least "numberOfBytes" long, never ever a null pointer
	*  @param[in] numberOfBytes
	*    Number of bytes to read from the source buffer and write into the file, it's the callers responsibility that this number of byte is correct
	*/
	virtual void write(const void* sourceBuffer, size_t numberOfBytes) = 0;

	//[-------------------------------------------------------]
	//[ Debug                                                 ]
	//[-------------------------------------------------------]
#if SE_DEBUG
	/**
	*  @brief
	*    Get debug filename
	*
	*  @return
	*    The debug filename, can be a null pointer, do not free the memory
	*/
	[[nodiscard]] virtual const char* getDebugFilename() const = 0;
#endif

protected:
	inline IFile()
	{
		// Nothing here
	}

	inline virtual ~IFile()
	{
		// Nothing here
	}

	explicit IFile(const IFile&) = delete;
	IFile& operator=(const IFile&) = delete;
};