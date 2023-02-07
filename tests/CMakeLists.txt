find_package(Catch2 3 REQUIRED)
add_executable(Tests mathfunctests.cpp importertests.cpp exportertests.cpp geoobjtests.cpp)
target_link_libraries(Tests PRIVATE Catch2::Catch2WithMain RenderToy)

add_test(NAME Tests COMMAND Tests)

include(CTest)
include(Catch)
catch_discover_tests(Tests)
