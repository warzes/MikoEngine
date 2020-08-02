#pragma once

struct Version
{
	Version() = delete;
	Version(unsigned major, unsigned minor, unsigned revision, const char *name)
		: Major(major)
		, Minor(minor)
		, Revision(revision)
		, Name(name)
	{
	}
	Version(const Version &v)
		: Major(v.Major)
		, Minor(v.Minor)
		, Revision(v.Revision)
		, Name(v.Name)
	{
	}

	Version operator=(const Version &v) noexcept 
	{		
		return Version(v);
	}

	unsigned GetVersion() const
	{
		return Major * 1000000 + Minor * 1000 + Revision;
	}

	const std::string& GetVersionString() const
	{
		return std::to_string(Major) + "." + std::to_string(Minor) + "." + std::to_string(Revision);
	}

	// main core
	const unsigned Major;
	// minor features, major updates
	const unsigned Minor;
	// minor bug fixes, alterations, refactors, updates
	const unsigned Revision;
	// Name
	const char *const Name;
};

static inline Version EngineVersion = { SE_VERSION_MAJOR, SE_VERSION_MINOR, SE_VERSION_REVISION, SE_ENGINE_NAME };