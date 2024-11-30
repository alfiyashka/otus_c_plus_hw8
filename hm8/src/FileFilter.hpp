#ifndef __FILE_FILTER__
#define __FILE_FILTER__

#include "FileMaskHelper.hpp"
#include <string>

class FileFilter
{
    const FileMaskHelper m_fileMaskHelper;
    const int m_minFileSize;

public:
    FileFilter(const std::string& fileMask, const int minFileSize)
    : m_fileMaskHelper(fileMask), m_minFileSize(minFileSize)
    {}

    bool filter(const std::string& fullFilePath) const;


};

#endif