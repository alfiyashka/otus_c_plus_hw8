#ifndef __CMD_PARAMS_HELPER__
#define __CMD_PARAMS_HELPER__

#include <vector>
#include <string>
#include "InputParameters.hpp"

#include "boost/unordered_set.hpp"
#include "boost/container/vector.hpp"


class CmdParamsParser
{
private:
  boost::container::vector<std::string> m_dirs;
  boost::unordered_set<std::string> m_excludedDirs;
  bool m_recursively;
  int m_minSize;
  std::string m_filemask;
  int m_blockSize;
  std::string m_hashFunctionName;

  CmdParamsParser(const CmdParamsParser &) = delete;
  CmdParamsParser& operator= (const CmdParamsParser&) = delete;
  CmdParamsParser(CmdParamsParser&&) = delete;
  CmdParamsParser& operator= (CmdParamsParser&&) = delete;
  
  CmdParamsParser(): m_recursively(true), m_minSize(1), m_filemask("*"), m_blockSize(1)
  {

  }
  ~CmdParamsParser() {}    
public:

    static CmdParamsParser& Instance()
    {
        static CmdParamsParser s;
        return s;
    }

    void parseCmdParams(int argc, const char *argv[]);

    InputParameters getParsedParameters() const noexcept;
};


#endif