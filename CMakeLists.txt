cmake_minimum_required (VERSION 3.21 FATAL_ERROR)
file(STRINGS "VERSION" VERSION_STR)
project(teiacare_prometheus_client LANGUAGES CXX VERSION ${VERSION_STR})

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_SOURCE_DIR}/build/modules)

include(GNUInstallDirs)
include(CMakePrintHelpers)
include(compiler_info)
include(options)
include(warnings)
include(unit_tests)
include(examples)
enable_testing()

add_subdirectory(prometheus_client)
