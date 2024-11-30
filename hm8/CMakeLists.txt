cmake_minimum_required(VERSION 3.28.3)


set(PATCH_VERSION "1" CACHE INTERNAL "PATCH_VERSION" )
set(PROJECT_VERSION 0.0.${PATCH_VERSION})


project(bayan VERSION ${PROJECT_VERSION} LANGUAGES C CXX)


find_package(
  Boost  REQUIRED  COMPONENTS 
             filesystem
             program_options
             )


configure_file(version.h.in version.h)



include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_BINARY_DIR}  ${Boost_INCLUDE_DIRS} )

set ( SRC_HEADER
  src/CmdParamsHelper.hpp
  src/HashHelper.hpp
  src/CmdParamsHelper.hpp
  src/DirectoryParser.hpp
  src/FileMaskHelper.hpp
  src/FileFilter.hpp
)
set ( SRC
  src/CmdParamsHelper.cpp
  src/HashHelper.cpp
  src/DirectoryParser.cpp
  src/FileMaskHelper.cpp
  src/FileFilter.cpp
)



add_executable(bayan ${SRC_HEADER} ${SRC} main.cpp)

target_link_libraries(
  bayan
  LINK_PUBLIC Boost::filesystem
              Boost::program_options)

link_directories(${Boost_LIBRARY_DIRS})

set_property(TARGET bayan PROPERTY CXX_STANDARD 20)


include("functions.cmake")
generate_packages(${PROJECT_NAME})

