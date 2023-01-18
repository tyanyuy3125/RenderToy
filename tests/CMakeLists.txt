#
# CMakeLists.txt for unit testing module
# File created on 2023/1/10
# Last edited on 2023/1/11
# Tianyu Huang <tianyu@illumiart.net>
#

# Include(FetchContent)
# FetchContent_Declare(
#   Catch2
#   GIT_REPOSITORY https://github.com/catchorg/Catch2.git
#   GIT_TAG        v3.2.1 # or a later release
# )
# FetchContent_MakeAvailable(Catch2)

find_package(Catch2 REQUIRED)

add_executable(Tests mathfunctests.cpp importertests.cpp exportertests.cpp geoobjtests.cpp)
target_link_libraries(Tests PRIVATE Catch2::Catch2WithMain OpenPT)

add_test(NAME Tests COMMAND Tests)

include(CTest)
include(Catch)
catch_discover_tests(Tests)