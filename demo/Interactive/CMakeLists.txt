#
# CMakeLists.txt for CornellBox Demo
# File created on 2023/1/16
# Tianyu Huang <tianyu@illumiart.net>
#

find_package(X11 REQUIRED)

add_executable(Interactive main.cpp)
target_link_libraries(Interactive PRIVATE OpenPT)
target_link_libraries(Interactive PRIVATE X11)
configure_file(cornellbox.obj ${EXECUTABLE_OUTPUT_PATH} cornellbox.obj COPYONLY)
