# Include(FetchContent)
# FetchContent_Declare(
#   Catch2
#   GIT_REPOSITORY https://github.com/catchorg/Catch2.git
#   GIT_TAG        v3.2.1 # or a later release
# )
# FetchContent_MakeAvailable(Catch2)

set(CATCH2_SOURCE_DIR ${PROJECT_SOURCE_DIR}/lib/Catch2)

find_package(Catch2 3 REQUIRED)
add_executable(Tests mathfunctests.cpp importertests.cpp exportertests.cpp geoobjtests.cpp)
target_link_libraries(Tests PRIVATE Catch2::Catch2WithMain RenderToy)

add_test(NAME Tests COMMAND Tests)

list(APPEND CMAKE_MODULE_PATH ${CATCH2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)
catch_discover_tests(Tests)