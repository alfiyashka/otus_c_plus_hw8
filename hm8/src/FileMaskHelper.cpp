#include "FileMaskHelper.hpp"

#include <boost/regex.hpp>

 bool FileMaskHelper::matches(const std::string& data) const
 {
    if (m_mask.empty() || m_mask == "*")
    {
        // for * - The repeat operator "*" cannot start a regular expression.  The error occurred while parsing the regular expression: '>>>HERE>>>*'
        return true;
    }

    try
    {
        static const boost::regex regex(m_mask);
        boost::smatch results;
        if (boost::regex_match(data, results, regex))
        {
            return true;
        }
        return false;
    }
    catch (const std::exception& e)
    {
        const std::string error = std::string("Cannot check file name by filemask [") + m_mask + "]. Error: " + e.what();
        throw std::runtime_error(error.c_str());
    }
 }