#include "FileFilter.hpp"

#include "boost/filesystem/path.hpp"
#include <boost/filesystem.hpp>


bool FileFilter::filter(const std::string& fullFilePath) const
{
    if (fullFilePath.empty())
    {
        return true;
    }
    boost::filesystem::path p(fullFilePath);
    const auto filename = p.filename().c_str();

    if (!m_fileMaskHelper.matches(filename))
    {
        return true;
    }

    const auto currentFileSize = boost::filesystem::file_size(p);
    if (currentFileSize < m_minFileSize)
    {
        return true;
    }
    return false;
}