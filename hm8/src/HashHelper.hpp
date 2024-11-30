#ifndef __HASH_HELPER__
#define __HASH_HELPER__

#include <vector>
#include <string>

#include <boost/bimap/bimap.hpp>
#include <boost/assign.hpp>
#include <boost/algorithm/string.hpp>


using namespace boost::bimaps;
using namespace boost;


class HashHelper
{
    enum EHashFunction
    {
        MD5,
        CRC32,
        UNSUPPORTED
    };
    using bm_type = boost::bimaps::bimap<EHashFunction, std::string>;
    inline static const bm_type HashFunctionMap =
        boost::assign::list_of<bm_type::relation>
            (MD5, "MD5")
            (CRC32, "CRC32");


    const std::string m_hashFunctionName;

    EHashFunction getHashFunction() const;
    uint crc32hash(const char* data, const int len) const;
    uint md5hash(const char* data, const int len) const;
public:
    HashHelper(const std::string& hashName)
       : m_hashFunctionName(boost::to_upper_copy<std::string>(hashName))
    {

    }

    uint calculate(const char* data, const int len) const;


};


#endif