#
# CMakeLists.txt for CornellBox Demo
# File created on 2023/1/16
# Tianyu Huang <tianyu@illumiart.net>
#

add_executable(CornellBox main.cpp)
target_link_libraries(CornellBox PRIVATE MathFunc Importer)
configure_file(testfile.in ${EXECUTABLE_OUTPUT_PATH} testfile.in COPYONLY)

