#
# CMakeLists.txt for CornellBox Demo
# File created on 2023/1/17
# Tianyu Huang <tianyu@illumiart.net>
#

add_executable(CornellBox main.cpp)
target_link_libraries(CornellBox PRIVATE OpenPT)
configure_file(cornellbox.obj ${EXECUTABLE_OUTPUT_PATH} cornellbox.obj COPYONLY)
