#
# CMakeLists.txt for CornellBox Demo
# File created on 2023/1/17
# Tianyu Huang <tianyu@illumiart.net>
#

add_executable(CubeDemo main.cpp)
target_link_libraries(CubeDemo PRIVATE OpenPT)
configure_file(cornellbox.obj ${EXECUTABLE_OUTPUT_PATH} cornellbox.obj COPYONLY)
