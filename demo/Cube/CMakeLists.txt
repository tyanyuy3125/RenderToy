#
# CMakeLists.txt for CornellBox Demo
# File created on 2023/1/17
# Tianyu Huang <tianyu@illumiart.net>
#

add_executable(CubeDemo main.cpp)
target_link_libraries(CubeDemo PRIVATE OpenPT)
configure_file(cube.obj ${EXECUTABLE_OUTPUT_PATH} cube.obj COPYONLY)
configure_file(sphere.obj ${EXECUTABLE_OUTPUT_PATH} sphere.obj COPYONLY)
