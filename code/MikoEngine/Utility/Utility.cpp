#include "stdafx.h"
#include "Utility.h"
#include "Core/Logger.h"

#ifdef WIN32
#    include <direct.h>
#    define GetCurrentDir _getcwd
#    define ChangeWorkingDir _chdir
#else
#    include <unistd.h>
#    define GetCurrentDir getcwd
#    define ChangeWorkingDir chdir
#endif

#ifdef __APPLE__
#    include <mach-o/dyld.h>
#endif

namespace utility
{
    static std::string g_exe_path = "";

    // -----------------------------------------------------------------------------------------------------------------------------------

    std::string path_for_resource(const std::string& resource)
    {
        std::string exe_path = executable_path();
#ifdef __APPLE__
        return exe_path + "/Contents/Resources/" + resource;
#else
        return exe_path + "/" + resource;
#endif
    }

    // -----------------------------------------------------------------------------------------------------------------------------------

#ifdef WIN32
    std::string executable_path()
    {
        if ( g_exe_path == "" )
        {
            char buffer[1024];
            GetModuleFileNameA(NULL, &buffer[0], 1024);
            g_exe_path = buffer;
            g_exe_path = path_without_file(g_exe_path);
        }

        return g_exe_path;
    }
#elif __APPLE__
    std::string executable_path()
    {
        if ( g_exe_path == "" )
        {
            char path[1024];
            uint32_t size = sizeof(path);
            if ( _NSGetExecutablePath(path, &size) == 0 )
            {
                g_exe_path = path;

                // Substring three times to get back to root path.
                for ( int i = 0; i < 3; i++ )
                {
                    std::size_t found = g_exe_path.find_last_of("/");
                    g_exe_path = g_exe_path.substr(0, found);
                }
            }
        }

        return g_exe_path;
    }
#else
    std::string executable_path()
    {
        if ( g_exe_path == "" )
        {
        }

        return g_exe_path;
    }
#endif

    // -----------------------------------------------------------------------------------------------------------------------------------

    std::string current_working_directory()
    {
        char buffer[FILENAME_MAX];

        if ( !GetCurrentDir(buffer, sizeof(buffer)) )
            return "";

        buffer[sizeof(buffer) - 1] = '\0';
        return std::string(buffer);
    }

    // -----------------------------------------------------------------------------------------------------------------------------------

    void change_current_working_directory(std::string path)
    {
        ChangeWorkingDir(path.c_str());
    }

    // -----------------------------------------------------------------------------------------------------------------------------------

    std::string path_without_file(std::string filepath)
    {
#ifdef WIN32
        std::replace(filepath.begin(), filepath.end(), '\\', '/');
#endif
        std::size_t found = filepath.find_last_of("/\\");
        std::string path = filepath.substr(0, found);
        return path;
    }

    // -----------------------------------------------------------------------------------------------------------------------------------

    std::string file_extension(std::string filepath)
    {
        std::size_t found = filepath.find_last_of(".");
        std::string ext = filepath.substr(found + 1, filepath.size());
        return ext;
    }

    // -----------------------------------------------------------------------------------------------------------------------------------

    std::string file_name_from_path(std::string filepath)
    {
        std::size_t slash = filepath.find_last_of("/");

        if ( slash == std::string::npos )
            slash = 0;
        else
            slash++;

        std::size_t dot = filepath.find_last_of(".");
        std::string filename = filepath.substr(slash, dot - slash);

        return filename;
    }

    // -----------------------------------------------------------------------------------------------------------------------------------

    bool read_text(std::string path, std::string& out)
    {
        std::ifstream file;
        file.open(path);

        if ( file.is_open() )
        {
            file.seekg(0, std::ios::end);
            out.reserve(file.tellg());
            file.seekg(0, std::ios::beg);
            out.assign((std::istreambuf_iterator<char>(file)),
                std::istreambuf_iterator<char>());

            return true;
        }
        else
            return false;
    }

    // -----------------------------------------------------------------------------------------------------------------------------------

    template <typename T>
    bool contains(const std::vector<T>& vec, const T& obj)
    {
        for ( auto& e : vec )
        {
            if ( e == obj )
                return true;
        }

        return false;
    }

    // -----------------------------------------------------------------------------------------------------------------------------------

    bool read_shader(const std::string& path, std::string& out, std::vector<std::string> defines)
    {
        std::string og_source;

        if ( !utility::read_text(path, og_source) )
            return false;

        if ( defines.size() > 0 )
        {
            for ( auto define : defines )
                out += "#define " + define + "\n";

            out += "\n";
        }

        return preprocess_shader(path, og_source, out);
    }

    // -----------------------------------------------------------------------------------------------------------------------------------

    std::string header_guard_from_path(const std::string& path)
    {
        std::string out = file_name_from_path(path);
        std::transform(out.begin(), out.end(), out.begin(), ::toupper);

        return out + "_H";
    }

    // -----------------------------------------------------------------------------------------------------------------------------------

    bool preprocess_shader(const std::string& path, const std::string& src, std::string& out)
    {
        std::istringstream       stream(src);
        std::string              line;
        std::vector<std::string> included_headers;

        while ( std::getline(stream, line) )
        {
            if ( line.find("#include") != std::string::npos )
            {
                size_t      start = line.find_first_of("<") + 1;
                size_t      end = line.find_last_of(">");
                std::string include_path = line.substr(start, end - start);

                std::string path_to_shader = "";
                size_t      slash_pos = path.find_last_of("/");

                if ( slash_pos != std::string::npos )
                    path_to_shader = path.substr(0, slash_pos + 1);

                std::string include_source;

                std::string og_source;

                if ( !utility::read_text(path_to_shader + include_path, og_source) )
                    return false;

                if ( !preprocess_shader(path_to_shader + include_path, og_source, include_source) )
                {
                    SE_LOG_ERROR("Included file <" + include_path + "> cannot be opened!");
                    return false;
                }
                if ( contains(included_headers, include_path) )
                    SE_LOG_WARNING("Header <" + include_path + "> has been included twice!");
                else
                {
                    included_headers.push_back(include_path);

                    std::string header_guard = header_guard_from_path(include_path);

                    out += "#ifndef ";
                    out += header_guard;
                    out += "\n#define ";
                    out += header_guard;
                    out += "\n\n";
                    out += include_source + "\n\n";
                    out += "#endif\n\n";
                }
            }
            else
                out += line + "\n";
        }

        return true;
    }
} // namespace utility