# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "/Users/jenyatokarzhevsky/CLionProjects/CatGenieEmulator/cmake-build-debug/_deps/picotool-src")
  file(MAKE_DIRECTORY "/Users/jenyatokarzhevsky/CLionProjects/CatGenieEmulator/cmake-build-debug/_deps/picotool-src")
endif()
file(MAKE_DIRECTORY
  "/Users/jenyatokarzhevsky/CLionProjects/CatGenieEmulator/cmake-build-debug/_deps/picotool-build"
  "/Users/jenyatokarzhevsky/CLionProjects/CatGenieEmulator/cmake-build-debug/_deps"
  "/Users/jenyatokarzhevsky/CLionProjects/CatGenieEmulator/cmake-build-debug/picotool/tmp"
  "/Users/jenyatokarzhevsky/CLionProjects/CatGenieEmulator/cmake-build-debug/picotool/src/picotoolBuild-stamp"
  "/Users/jenyatokarzhevsky/CLionProjects/CatGenieEmulator/cmake-build-debug/picotool/src"
  "/Users/jenyatokarzhevsky/CLionProjects/CatGenieEmulator/cmake-build-debug/picotool/src/picotoolBuild-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/jenyatokarzhevsky/CLionProjects/CatGenieEmulator/cmake-build-debug/picotool/src/picotoolBuild-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/jenyatokarzhevsky/CLionProjects/CatGenieEmulator/cmake-build-debug/picotool/src/picotoolBuild-stamp${cfgdir}") # cfgdir has leading slash
endif()
