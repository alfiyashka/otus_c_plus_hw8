#include "DirectoryParser.hpp"
#include "iostream"


#include "boost/filesystem/fstream.hpp"

using namespace boost::filesystem;

bool DirectoriesParser::isDirectory(const path& p) const
{
    system::error_code error;
    return is_regular_file(p, error) == false;
}

bool DirectoriesParser::isExcludedDirectory(const path& p) const
{
    if (m_excludedDirs.empty())
    {
        return false;
    }
    const auto& filename = p.string();
    auto foundIter = m_excludedDirs.find(filename);
    bool found = foundIter != m_excludedDirs.end();
    if (!found && filename[filename.length() -1] != '/')
    {
        const std::string filenameWithSlash = filename + "/";
        auto foundWithSlash = m_excludedDirs.find(filenameWithSlash);
        return foundWithSlash != m_excludedDirs.end();
    }
    return found;
}

bool DirectoriesParser::needFilterOutFile(const path& p) const
{
    return !isDirectory(p) && m_fileFilter.filter(p.string());
}


bool DirectoriesParser::needFilterOutChildPath(const path& p) const
{
   return !m_recursively && isDirectory(p)
    || isExcludedDirectory(p);
}

bool DirectoriesParser::needFilterOutRootPath(const path& p) const
{
   return isExcludedDirectory(p) || !isDirectory(p);
}

bool DirectoriesParser::isFileExists(const path& p) const
{
    return boost::filesystem::exists(p);
}

void DirectoriesParser::readFile(const path& p, boost::container::vector<uint>& cacheVector)
{
    ifstream ifs(p, std::ios_base::binary);
    char* data = new char[m_blockSize];
    while(!ifs.read(data, m_blockSize).eof())
    {
        const int readed = ifs.gcount();
        if (readed < m_blockSize)
        {
            std::memset(data + readed, 0, m_blockSize - readed);
        }
        else if (readed > m_blockSize)
        {
            throw std::runtime_error("The readed data length is greater than block size ");
        }

        const auto hash = m_hashHelper.calculate(data, m_blockSize);
        cacheVector.push_back(hash);
    }

    delete[] data;

    ifs.close();
}

boost::container::vector<uint>& DirectoriesParser::getHashes(const path& p)
{
    const auto filename = p.string();
    const auto found = m_checksumsCache.find(filename);
    if (found != m_checksumsCache.end())
    {
        return (*found).second;
    }
    else
    {
        m_checksumsCache[filename] = boost::container::vector<uint>();
        auto& hashes = m_checksumsCache[filename];
        readFile(p, hashes);
        return hashes;
    }
}


bool DirectoriesParser::equalFiles(const boost::container::vector<uint>& hashes1, const boost::container::vector<uint>& hashes2)
{
    if (hashes1.size() != hashes2.size())
    {
        return false;
    }
    const int hashesSize = hashes1.size();
    for (int i = 0; i < hashesSize; ++i)
    {
        if (hashes1[i] != hashes2[i])
        {
            return false;
        }
    }
    return true;
}


bool DirectoriesParser::tryDetectDuplicate(const path& p1, const path& p2)
{
    const auto& hashes1 = getHashes(p1);
    const auto& hashes2 = getHashes(p2);
    if (equalFiles(hashes1, hashes2))
    {
        auto found = m_resultMap.find(p1.string());
        if (found != m_resultMap.end())
        {
            found->second.insert(p2.string());
            
            auto found2 = m_resultMap.find(p2.string());
            if (found2 == m_resultMap.end())
            {
                boost::unordered_set<std::string>filenames;
                filenames.insert(p1.string());
                filenames.insert(p2.string());
                m_resultMap.insert(ResultMap::value_type(p2.string(), filenames));
            }
            else
            {
                found2->second.insert(p1.string());
            }
        }
        else
        {
            boost::unordered_set<std::string>filenames;
            filenames.insert(p1.string());
            filenames.insert(p2.string());
            
            m_resultMap.insert(ResultMap::value_type(p1.string(), filenames));

            auto found2 = m_resultMap.find(p2.string());
            if (found2 == m_resultMap.end())
            {
                m_resultMap.insert(ResultMap::value_type(p2.string(), filenames));
            }
            else
            {
                found2->second.insert(p1.string());
            }
        }
        return true;
    }
    else
    {
        m_resultMap.insert(ResultMap::value_type(p2.string(),
            boost::unordered_set<std::string>{p2.string()}));
        m_resultMap.insert(ResultMap::value_type(p1.string(),
            boost::unordered_set<std::string>{p1.string()}));
    }
    return false;
}


int DirectoriesParser::collectFilesToParse()
{
    if (m_dirs.empty())
    {
        std::cerr<<"Directories to parse are not defined"<<std::endl;
        return 0;
    }
    for (auto dir : m_dirs)
    {
        const path rootPath(dir);
        if (!isFileExists(dir))
        {
            std::cout<<"The directory '" << dir.c_str() << "' does not exist and it have been ignored";
            continue;
        }
        if (needFilterOutRootPath(rootPath))
        {
            continue;
        }

        recursive_directory_iterator dirIter(dir), end;
        for (; dirIter != end; ++dirIter)
        {
            const auto& iPath = dirIter->path();
            if (needFilterOutChildPath(iPath))
            {
                dirIter.disable_recursion_pending(); // don't recurse into this directory.
                continue;
            }

            if (isDirectory(iPath))
            {
                continue;
            }

            if (needFilterOutFile(iPath))
            {
                continue;
            }
            m_filePathsToParse.emplace_back(iPath.string());
        }       
    }
    return m_filePathsToParse.size();

}

int DirectoriesParser::findDublicates()
{
    try
    {
        const int collectedFilesCount = collectFilesToParse();
        if (collectedFilesCount == 0)
        {
            return -1;
        }
        std::string currentFileToParse;
        for (auto iFile : m_filePathsToParse)
        {
            if (currentFileToParse.empty())
            {
                currentFileToParse = iFile;
                continue;
            }

            findDublicatesOfFile(currentFileToParse);

            currentFileToParse = iFile;
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << "Cannot find dublicates because of following error: " << e.what() << '\n';
    }
    
    return 0;
}

void DirectoriesParser::findDublicatesOfFile(const std::string& checkWith)
{
    for (auto iFile : m_filePathsToParse)
    {
        if (checkWith == iFile)
        {
            continue;
        }
        tryDetectDuplicate(path(checkWith), path(iFile));
    }
}

void DirectoriesParser::printResults()
{
    if (m_cacheResultSet.empty())
    {
        std::transform(m_resultMap.cbegin(), m_resultMap.cend(),
               std::inserter(m_cacheResultSet, m_cacheResultSet.begin()),
               [](const ResultMap::value_type& key_value)
               { return key_value.second; });
    }


    for (auto iRes: m_cacheResultSet)
    {
        for (auto iDublicate : iRes)
        {
            std::cout<<iDublicate << std::endl;
        }
        std::cout << std::endl;
    }

}