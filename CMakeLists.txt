#
# CMakeLists.txt for the whole OpenPT project
# File created on 2023/1/10
# Tianyu Huang <tianyu@illumiart.net>
#

cmake_minimum_required(VERSION 3.16.3)
project(OPENPT LANGUAGES CXX VERSION 0.1)

set(cmake_cxx_standard 17)
set(cmake_cxx_standard_required True)

add_subdirectory(src)

# Unit test related
add_subdirectory(tests)
