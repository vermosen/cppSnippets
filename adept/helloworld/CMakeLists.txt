# CMakeList.txt : CMake project for smoothy, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.12)

if (CMAKE_CONF)
	set(CMAKE_BUILD_TYPE "${CMAKE_CONF}")
endif()

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
endif()

string(TOLOWER "${CMAKE_BUILD_TYPE}" cmake_build_type_tolower)
if(    NOT cmake_build_type_tolower STREQUAL "debug"
   AND NOT cmake_build_type_tolower STREQUAL "release")
   message(FATAL_ERROR "Unknown build type \"${CMAKE_BUILD_TYPE}\". Allowed values are Debug, Release (case-insensitive).")
endif()

include(GNUInstallDirs)
include(CheckCXXCompilerFlag)

# general configuration
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# find adept
set (LIB_ADEPT "libadept.so")
find_library(LIB_ADEPT NAMES adept PATHS "/usr/local/lib")

# add the project sources
file(GLOB source_files
            "*.h"
            "*.cpp"
    )

# Add source to this project's executable.
add_executable (helloworld_${CMAKE_CONF} ${source_files})
SET(CMAKE_EXE_LINKER_FLAGS "-ladept")
