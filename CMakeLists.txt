cmake_minimum_required(VERSION 3.17)
cmake_policy(SET CMP0100 NEW) # handle .hh files

enable_testing()

project(clap-plugins C CXX)

set(CMAKE_AUTOMOC ON)

if(${CMAKE_CXX_COMPILER_ID} MATCHES "AppleClang|Clang|GNU")
  add_compile_options(-fvisibility=hidden -fvisibility-inlines-hidden
    -Werror=return-type -Werror=unused-result -Wno-unused-parameter
    -Wuninitialized -Wvla -Wmultichar -Werror=non-virtual-dtor -Woverloaded-virtual)
endif()

if(${CMAKE_CXX_COMPILER_ID} MATCHES "AppleClang|Clang")
  add_compile_options(-Wmost -Wsuper-class-method-mismatch)
endif()

find_package(clap REQUIRED)
find_package(clap-helpers REQUIRED)
find_package(PkgConfig)
pkg_search_module(YAS REQUIRED yas)
include_directories(${YASINCLUDE_DIRS})

add_subdirectory(plugins)
