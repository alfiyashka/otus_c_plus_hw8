#ifndef __DIRECTORIES_PARSER__
#define __DIRECTORIES_PARSER__

#include <vector>
#include <string>
#include <algorithm>
#include <unordered_set>
#include <set>

#include "InputParameters.hpp"
#include "FileFilter.hpp"
#include "HashHelper.hpp"

#include "boost/filesystem/path.hpp"
#include "boost/filesystem.hpp"
#include "boost/unordered_set.hpp"
#include "boost/unordered_map.hpp"
#include "boost/container/vector.hpp"
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>

using namespace boost::filesystem;
using boost::multi_index_container;
using namespace boost::multi_index;

using FilesHashType = boost::unordered_map<std::string, boost::container::vector<uint> >;

using ResultMap = boost::unordered_map<std::string, boost::unordered_set<std::string> >;

struct HashFilenames
{
  hash<std::string> hasher;
  std::size_t operator()(boost::unordered_set<std::string> const &set) const
  {
    std::size_t seed = set.size();
    for (auto& i : set)
    {
      seed += hasher(i);
    }
    return seed;
  }
};

using ResultSet = boost::unordered_set<boost::unordered_set<std::string>, HashFilenames>;

class DirectoriesParser
{
    const FileFilter m_fileFilter;
    const HashHelper m_hashHelper;
    const int m_blockSize;
    const bool m_recursively;
    const boost::container::vector<std::string> m_dirs;
    const boost::unordered_set<std::string> m_excludedDirs;
    
    FilesHashType m_checksumsCache;

    boost::container::vector<std::string> m_filePathsToParse;
    
    ResultMap m_resultMap;
    ResultSet m_cacheResultSet;
  
    bool isDirectory(const path& p) const;
    bool isExcludedDirectory(const path& p) const;
    bool needFilterOutRootPath(const path& p) const;
    bool needFilterOutChildPath(const path& p) const;
    bool needFilterOutFile(const path& p) const;
    void readFile(const path& p, boost::container::vector<uint>& cacheVector);

    boost::container::vector<uint>& getHashes(const path& p);
    bool equalFiles(const boost::container::vector<uint>& h1, const boost::container::vector<uint>& h2);
    bool tryDetectDuplicate(const path& p1, const path& p2);

    bool isFileExists(const path& p) const;

    int collectFilesToParse();

    void findDublicatesOfFile(const std::string& checkWith);

public:
    DirectoriesParser(const InputParameters &inputParameters)
        : m_fileFilter(inputParameters.m_filemask, inputParameters.m_minSize),
          m_hashHelper(inputParameters.m_hashFunctionName),
          m_blockSize(inputParameters.m_blockSize),
          m_recursively(inputParameters.m_recursively),
          m_dirs(inputParameters.m_dirs),
          m_excludedDirs(inputParameters.m_excludedDirs)
    {
    }
    DirectoriesParser(const DirectoriesParser &parser)
        : m_fileFilter(parser.m_fileFilter),
          m_hashHelper(parser.m_hashHelper),
          m_blockSize(parser.m_blockSize),
          m_recursively(parser.m_recursively),
          m_dirs(parser.m_dirs),
          m_excludedDirs(parser.m_excludedDirs)
    {
    }

    int findDublicates();

    void printResults();
};


#endif