#
# CMakeLists.txt for CornellBox Demo
# File created on 2023/1/17
# Tianyu Huang <tianyu@illumiart.net>
#

add_executable(TestRender main.cpp)
target_link_libraries(TestRender PRIVATE OpenPT)
