#include "HashHelper.hpp"

#include <boost/crc.hpp>
#include <boost/uuid/detail/md5.hpp>
#include <boost/algorithm/string.hpp>

using boost::uuids::detail::md5;

HashHelper::EHashFunction HashHelper::getHashFunction() const
{
    if (m_hashFunctionName.empty())
    {
        throw std::runtime_error("Hash function is undefined");
    }
    const auto hash = HashFunctionMap.right.find(m_hashFunctionName);
    if (hash == HashFunctionMap.right.end())
    {
        throw std::runtime_error(std::string("Hash function is not supported. Current value is:" + m_hashFunctionName).c_str());
    }
    return hash->get_left();
}

uint HashHelper::crc32hash(const char* data, const int len) const
{
    boost::crc_32_type result;
    result.process_bytes(data, len);
    return result.checksum();
}

uint HashHelper::md5hash(const char* data, const int len) const
{
    md5 hash;
    md5::digest_type digest;

    hash.process_bytes(data, len);
    hash.get_digest(digest);
    return (uint)*digest;
    
}

uint HashHelper::calculate(const char* data, const int len) const
{
    
    const auto hash = getHashFunction();
    switch(hash)
    {
        case MD5:
            return md5hash(data, len);
        case CRC32:
            return crc32hash(data, len);
        default:
            throw std::runtime_error(std::string("Hash function is not implemented. Current value is:" + m_hashFunctionName).c_str());
    }
    
}