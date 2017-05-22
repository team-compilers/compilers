#pragma once

#include <string>


std::string JoinPath( const std::string& pathBegin, const std::string& pathEnd );
bool ExistsDirectory( const std::string& path );
bool CreateDirectory( const std::string& path );
