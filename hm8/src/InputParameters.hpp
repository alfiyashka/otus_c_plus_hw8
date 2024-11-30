#ifndef __INPUT_PARAMETERS__
#define __INPUT_PARAMETERS__

#include <vector>
#include <string>
#include <algorithm>
#include "boost/unordered_set.hpp"
#include "boost/container/vector.hpp"

struct InputParameters
{
    const boost::container::vector<std::string> m_dirs;
    const boost::unordered_set<std::string> m_excludedDirs;
    const bool m_recursively;
    const int m_minSize;
    const std::string m_filemask;
    const int m_blockSize;
    const std::string m_hashFunctionName;
    InputParameters(const boost::container::vector<std::string> &dirs,
                    const boost::unordered_set<std::string> &excludedDirs,
                    const bool recursively,
                    const int minSize,
                    const std::string &filemask,
                    const int blockSize,
                    const std::string &hashFunctionName)
        : m_dirs(dirs),
          m_excludedDirs(excludedDirs),
          m_recursively(recursively),
          m_minSize(minSize),
          m_filemask(filemask),
          m_blockSize(blockSize),
          m_hashFunctionName(hashFunctionName)
    {

    }

    InputParameters(const InputParameters& param)
      : m_dirs(param.m_dirs),
        m_excludedDirs(param.m_excludedDirs),
        m_recursively(param.m_recursively),
        m_minSize(param.m_minSize),
        m_filemask(param.m_filemask),
        m_blockSize(param.m_blockSize),
        m_hashFunctionName(param.m_hashFunctionName)
    {
    }
    InputParameters& operator=(const InputParameters& param) = delete;
    InputParameters& operator=(InputParameters&& param) = delete;
    InputParameters(InputParameters&& param) = delete;
};

#endif