#include <iostream>
#include <cassert>
#include "src/CmdParamsHelper.hpp"

#include "src/DirectoryParser.hpp"

#include "src/HashHelper.hpp"


using namespace std;

int main(int argc, const char *argv[])
{
  if (argc < 2)
  {
    std::cerr << "The parameters are not defined" << std::endl;
    return -1;
  }
  auto &cmdParser = CmdParamsParser::Instance();
  cmdParser.parseCmdParams(argc, argv);

  DirectoriesParser dirParser(cmdParser.getParsedParameters());
  dirParser.findDublicates();
  dirParser.printResults();

  return 0;
}
