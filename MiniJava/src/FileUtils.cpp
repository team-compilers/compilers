#include <FileUtils.h>

#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <string>
#include <stdexcept>

std::string JoinPath( const std::string& pathBegin, const std::string& pathEnd ) {
    std::string path = pathBegin;
    if ( pathBegin.back() != '/' ) {
        path += '/';
    }
    return path + pathEnd;
}

bool ExistsDirectory( const std::string& path ) {
    DIR* dir = opendir( path.c_str() );
    bool notExists = !dir && errno == ENOENT;
    if ( dir ) {
        closedir( dir );
    }
    return !notExists;
}

bool CreateDirectory( const std::string& path ) {
    bool exists = ExistsDirectory( path );
    if ( !exists ) {
        int status = mkdir( path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH );
        if ( status != 0 ) {
            throw std::logic_error( strerror( errno ) );
        }
    }
    return exists;
}
