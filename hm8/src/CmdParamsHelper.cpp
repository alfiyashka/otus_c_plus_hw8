#include "CmdParamsHelper.hpp"
#include <boost/program_options.hpp>
#include <boost/program_options/options_description.hpp>
#include "boost/container/vector.hpp"

#include <iostream>

void CmdParamsParser::parseCmdParams(int argc, const char *argv[])
{
  namespace po = boost::program_options;
  po::options_description desc("");
  desc.add_options()
    ("dr", po::value<std::vector<std::string>>()->required(), "dr")
    ("exclDr", po::value<std::vector<std::string>>(), "excluded directories")
    ("r", po::value<int>()->default_value(1), "parse recursively or not")
    ("s", po::value<int>()->default_value(1), "minimal size of files to parse in bytes (default = 1)")
    ("m", po::value<std::string>()->default_value("(.*?)"), "file mask (default = '(.*?)')")
    ("b", po::value<int>()->default_value(1), "block size in bytes (default value = 1 byte)")
    ("h", po::value<std::string>()->default_value("md5"), "hash function");
    
  po::variables_map vm;
  po::positional_options_description positionals;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::store(po::command_line_parser(argc, argv).options(desc).run(), vm);

  po::notify(vm);
 
  if (vm.count("dr"));
  {
    for(std::string dir : vm["dr"].as<std::vector<std::string>>())
    {
      m_dirs.push_back(dir);
    }
    if (m_dirs.empty())
    {
      throw std::runtime_error ("The directoris to parse required!");
    }
  }

  if (vm.count("exclDr") != 0);
  {
    try
    {
      const auto exdir = vm["exclDr"].as<std::vector<std::string>>();
      if (!exdir.empty())
      {
        for(std::string dir : exdir)
        {
          m_excludedDirs.insert(dir);
        }
      }      
    }
    catch(const std::exception &)
    {
        //ignores, there is some bug.
        // If I don't set this option, vm.count("exclDr") always equal to 1 
    }
  }

  m_recursively = (bool)(boost::any_cast<int >(vm["r"].value()) != 0);
  m_minSize = boost::any_cast<int >(vm["s"].value());
  m_filemask = boost::any_cast<std::string >(vm["m"].value());
  m_blockSize = boost::any_cast<int >(vm["b"].value());
  m_hashFunctionName = boost::any_cast<std::string >(vm["h"].value());
}

InputParameters CmdParamsParser::getParsedParameters() const noexcept
{
  return InputParameters(m_dirs, m_excludedDirs,
    m_recursively, m_minSize, m_filemask, m_blockSize, m_hashFunctionName);
}