#
# CMakeLists.txt for unit testing module
# File created on 2023/1/10
# Tianyu Huang <tianyu@illumiart.net>
#

# Include(FetchContent)

# FetchContent_Declare(
#   Catch2
#   GIT_REPOSITORY https://github.com/catchorg/Catch2.git
#   GIT_TAG        v3.0.1 # or a later release
# )

# FetchContent_MakeAvailable(Catch2)

find_package(Catch2 REQUIRED)
add_executable(MathFuncTests mathfunctests.cpp)
target_link_libraries(MathFuncTests PRIVATE Catch2::Catch2WithMain MathFunc)

add_test(NAME MathFuncTests COMMAND MathFuncTests)

include(CTest)
include(Catch)
catch_discover_tests(MathFuncTests)