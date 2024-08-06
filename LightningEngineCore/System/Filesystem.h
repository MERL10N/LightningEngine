//
//  Filesystem.h
//  Metal-Tutorial
//
//  Created by Kian Marvi on 23/1/24.
//

#ifndef Filesystem_h
#define Filesystem_h
#include <string>
#include <cstdlib>
#include <sys/stat.h>
//#include "../GameControl/Settings.h"

#if defined (__linux__) || defined (__APPLE__)
    #include <unistd.h>
    #define GetCurrentDir getcwd
#elif _WIN32
    #include <direct.h>
    #define GetCurrentDir _getcwd
#endif

class FileSystem
{
private:
    typedef std::string(*Builder) (const std::string& path);

public:
    static std::string getPath(const std::string& path)
    {
        static std::string(*pathBuilder)(std::string const &) = getPathBuilder();

        std::string result = (*pathBuilder)(path);
        if (DoesFileExists(result) == true)
            return result;
        else
            return "";
    }

    static bool DoesFileExists(const std::string& name)
    {
        struct stat buffer;
        return (stat(name.c_str(), &buffer) == 0);
    }

private:
    static std::string const & getRoot()
    {
        char buff[FILENAME_MAX]; //create string buffer to hold path
        GetCurrentDir(buff, FILENAME_MAX);
        string current_working_dir(buff);

        static char const * envRoot = current_working_dir.c_str();
        static char const * givenRoot = (envRoot != nullptr ? envRoot : ".\\");
            //logl_root);
        static std::string root = (givenRoot != nullptr ? givenRoot : "");
        return root;
    }

    //static std::string(*foo (std::string const &)) getPathBuilder()
    static Builder getPathBuilder()
    {
        if (getRoot() != "")
            return &FileSystem::getPathRelativeRoot;
        else
            return &FileSystem::getPathRelativeBinary;
    }

    static std::string getPathRelativeRoot(const std::string& path)
    {
        return getRoot() + std::string("\\") + path;
    }

    static std::string getPathRelativeBinary(const std::string& path)
    {
        return "../../../" + path;
    }
};

#endif /* Filesystem_h */
