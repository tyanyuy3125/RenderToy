#
# CMakeLists.txt for CornellBox Demo
# File created on 2023/1/16
# Tianyu Huang <tianyu@illumiart.net>
#

add_executable(Maze main.cpp)
target_link_libraries(Maze PRIVATE OpenPT)
