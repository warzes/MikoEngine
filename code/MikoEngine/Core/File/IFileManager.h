#pragma once

class IFile;

//[-------------------------------------------------------]
//[ Global definitions                                    ]
//[-------------------------------------------------------]
typedef const char* VirtualFilename;		// UTF-8 virtual filename, the virtual filename scheme is "<mount point = project name>/<asset directory>/<asset name>.<file extension>" (example "Example/Mesh/Monster/Squirrel.mesh"), never ever a null pointer and always finished by a terminating zero
typedef const char* AbsoluteDirectoryName;	// UTF-8 absolute directory name (example: "c:/MyProject"), without "/" at the end, never ever a null pointer and always finished by a terminating zero
typedef const char* VirtualDirectoryName;	// UTF-8 virtual directory name (example: "MyProject/MyDirectory"), without "/" at the end, never ever a null pointer and always finished by a terminating zero


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract file manager interface
*
*  @remarks
*    Conventions:
*    - File and directory names are UTF-8 encoded
*    - Directory names have no "/"-slash at the end
*    - "/"-slash is used as separator
*    - The file manager interface works with virtual filenames to be compatible to libraries like PhysicsFS ( https://icculus.org/physfs/ ) by design
*    - Virtual filenames are constructed in way which is compatible to asset IDs and supports modding: "<mount point = project name>/<asset directory>/<asset name>.<file extension>"
*
*    For the examples were using the following directory structure
*    - "<root directory>/Binary/Windows_x64_Static"
*    - "<root directory>/Binary/DataPc"
*      - "<root directory>/Binary/DataPc/<Project>/<Asset Package>/<Asset Directory>/<Asset Filename>"
*    - "<root directory>/Binary/LocalData"
*    -> For end-user products, you might want to choose a local user data directory
*    -> In here we assume that the current directory has not been changed and still points to the directory the running executable is in (e.g. "<root directory>/Binary/Windows_x64_Static")
*
*  @note
*    - Also known as virtual file system (VFS)
*/
class IFileManager
{
public:
	enum class FileMode
	{
		READ,	// File read access
		WRITE	// File write access
	};
	enum class EnumerationMode
	{
		ALL,		// Enumerate files as well as directories
		FILES,		// Do only enumerate files
		DIRECTORIES	// Do only enumerate directories
	};
	static const constexpr char INVALID_CHARACTER = '?';	// Invalid character ( "Windows Dev Center" -> "Naming Files, Paths, and Namespaces" -> "File and Directory Names" -> "Naming Conventions" published on 31.05.2018 at https://docs.microsoft.com/de-de/windows/desktop/FileIO/naming-a-file ) which can e.g. be used as separator in debug names ("<filename>?[<attribute 0>][<attribute n>]")


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
public:
	/**
	*  @brief
	*    Return the absolute root directory
	*
	*  @return
	*    The absolute UTF-8 root directory, without "/" at the end
	*/
	[[nodiscard]] inline const std::string& getAbsoluteRootDirectory() const
	{
		return mAbsoluteRootDirectory;
	}


	//[-------------------------------------------------------]
	//[ Public virtual Renderer::IFileManager methods         ]
	//[-------------------------------------------------------]
public:
	/**
	*  @brief
	*    Return the name of the local data mount point were to write local data to
	*
	*  @return
	*    The UTF-8 name of the local data mount point were to write local data to (usually a user directory), if null pointer writing local data isn't allowed
	*
	*  @remarks
	*    Examples for local data
	*    - Log files, itself won't save log files
	*    - ImGui "ini"-files storing session information
	*    - Locally updated and saved pipeline state object cache in case the shipped one had cache misses
	*    - Renderer toolkit cache which is used to detect source data changes for incremental asset compilation instead of time consuming full asset compilation
	*/
	[[nodiscard]] virtual const char* getLocalDataMountPoint() const = 0;

	/**
	*  @brief
	*    Return a mounted directory
	*
	*  @param[in] mountPoint
	*    UTF-8 mount point (example: "MyProject"), never ever a null pointer and always finished by a terminating zero
	*
	*  @return
	*    Absolute UTF-8 name of the mounted directory,, null pointer on error
	*
	*  @see
	*    - "Renderer::IFileManager::mountDirectory()"
	*/
	[[nodiscard]] virtual const char* getMountPoint(const char* mountPoint) const = 0;

	/**
	*  @brief
	*    Mount a directory into the file manager
	*
	*  @param[in] absoluteDirectoryName
	*    Absolute UTF-8 name of the directory to mount (example: "c:/MyProject"), "" is equivalent to "/"
	*  @param[in] mountPoint
	*    UTF-8 mount point (example: "MyProject"), never ever a null pointer and always finished by a terminating zero
	*  @param[in] appendToPath
	*    "true" to append at the end of the search path, "false" to prepend (in case of overlapping files the new directory or archive is the preferred one)
	*
	*  @return
	*    "true" if all went fine, else "false"
	*/
	virtual bool mountDirectory(AbsoluteDirectoryName absoluteDirectoryName, const char* mountPoint, bool appendToPath = false) = 0;

	/**
	*  @brief
	*    Check whether or not a file exists
	*
	*  @param[in] virtualFilename
	*    UTF-8 virtual filename of the file to check for existence
	*
	*  @return
	*    "true" if the file does exist, else "false"
	*/
	[[nodiscard]] virtual bool doesFileExist(VirtualFilename virtualFilename) const = 0;

	/**
	*  @brief
	*    Enumerate files of a specified directory
	*
	*  @param[in] virtualDirectoryName
	*    Virtual UTF-8 name of the directory to enumerate the files of
	*  @param[in] enumerationMode
	*    Enumeration mode
	*  @param[out] virtualFilenames
	*    Receives the enumerated virtual UTF-8 filenames
	*/
	virtual void enumerateFiles(VirtualDirectoryName virtualDirectoryName, EnumerationMode enumerationMode, std::vector<std::string>& virtualFilenames) const = 0;

	/**
	*  @brief
	*    Map a virtual filename to an absolute filename
	*
	*  @param[in] fileMode
	*    File mode
	*  @param[in] virtualFilename
	*    UTF-8 virtual filename to map
	*
	*  @return
	*    Mapped UTF-8 absolute filename, empty string on error
	*/
	[[nodiscard]] virtual std::string mapVirtualToAbsoluteFilename(FileMode fileMode, VirtualFilename virtualFilename) const = 0;

	/**
	*  @brief
	*    Get the last modification time of a file
	*
	*  @param[in] virtualFilename
	*    Virtual UTF-8 filename to check
	*
	*  @return
	*    Last modified time of the file, -1 if it can't be determined
	*
	*  @remarks
	*    The modification time is returned as a number of seconds since the epoch
	*    (Jan 1, 1970). The exact derivation and accuracy of this time depends on
	*    the particular archiver. If there is no reasonable way to obtain this
	*    information for a particular archiver, or there was some sort of error,
	*    this function returns (-1).
	*/
	[[nodiscard]] virtual int64_t getLastModificationTime(VirtualFilename virtualFilename) const = 0;

	/**
	*  @brief
	*    Get the file size
	*
	*  @param[in] virtualFilename
	*    Virtual UTF-8 filename to check
	*
	*  @return
	*    File size, -1 if it can't be determined
	*/
	[[nodiscard]] virtual int64_t getFileSize(VirtualFilename virtualFilename) const = 0;

	/**
	*  @brief
	*    Create directories recursive
	*
	*  @param[in] virtualDirectoryName
	*    Virtual UTF-8 name of the directory to create, including all parent directories if necessary
	*
	*  @return
	*    "true" if all went fine or the directories already exist, else "false"
	*/
	virtual bool createDirectories(VirtualDirectoryName virtualDirectoryName) const = 0;

	/**
	*  @brief
	*    Open a file by using a virtual filename
	*
	*  @param[in] fileMode
	*    File mode
	*  @param[in] virtualFilename
	*    UTF-8 virtual filename of the file to open for reading
	*
	*  @return
	*    The file interface, can be a null pointer if horrible things are happening (total failure)
	*
	*  @note
	*    - ZIP files may be password-protected and each file may have a different password provided
	*      with the virtual filename syntax "<filename>$<password>" (e.g. "myfile$mypassword"); due
	*      to the negative loading time impact password secured asset packages are not supported
	*/
	[[nodiscard]] virtual IFile* openFile(FileMode fileMode, VirtualFilename virtualFilename) const = 0;

	/**
	*  @brief
	*    Close a file
	*
	*  @param[in] file
	*    File to close
	*/
	virtual void closeFile(IFile& file) const = 0;

protected:
	inline explicit IFileManager(const std::string& absoluteRootDirectory) :
		mAbsoluteRootDirectory(absoluteRootDirectory)
	{
		// Nothing here
	}

	inline virtual ~IFileManager()
	{
		// Nothing here
	}

	explicit IFileManager(const IFileManager&) = delete;
	IFileManager& operator=(const IFileManager&) = delete;

private:
	const std::string mAbsoluteRootDirectory;
};