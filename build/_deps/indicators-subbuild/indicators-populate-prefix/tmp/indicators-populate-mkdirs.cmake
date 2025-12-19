# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/data/workspace/github/cpp_snippet/build/_deps/indicators-src"
  "/data/workspace/github/cpp_snippet/build/_deps/indicators-build"
  "/data/workspace/github/cpp_snippet/build/_deps/indicators-subbuild/indicators-populate-prefix"
  "/data/workspace/github/cpp_snippet/build/_deps/indicators-subbuild/indicators-populate-prefix/tmp"
  "/data/workspace/github/cpp_snippet/build/_deps/indicators-subbuild/indicators-populate-prefix/src/indicators-populate-stamp"
  "/data/workspace/github/cpp_snippet/build/_deps/indicators-subbuild/indicators-populate-prefix/src"
  "/data/workspace/github/cpp_snippet/build/_deps/indicators-subbuild/indicators-populate-prefix/src/indicators-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/data/workspace/github/cpp_snippet/build/_deps/indicators-subbuild/indicators-populate-prefix/src/indicators-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/data/workspace/github/cpp_snippet/build/_deps/indicators-subbuild/indicators-populate-prefix/src/indicators-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
