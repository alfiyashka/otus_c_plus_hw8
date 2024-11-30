#ifndef __FILE_MASK_HELPER__
#define __FILE_MASK_HELPER__

#include <string>

class FileMaskHelper
{
    const std::string m_mask;
public:
    FileMaskHelper(const std::string& mask): m_mask(mask)
    {

    }

    bool matches(const std::string& data) const;

};

#endif