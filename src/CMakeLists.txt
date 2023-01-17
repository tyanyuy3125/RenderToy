#
# CMakeLists.txt for ./src
# File created on 2023/1/10
# Tianyu Huang <tianyu@illumiart.net>
#

add_library(MathFunc mathfunc.cpp mathfunc.h)
add_library(MeshObj meshobj.cpp meshobj.h)
add_library(Importer importer.cpp importer.h)
add_library(World world.cpp world.h)
# set(SOURCES
#     importer.cpp
#     mathfunc.cpp
#     meshobj.cpp
#     world.cpp
# )

# set(HEADERS_PUBLIC
#     mathfunc.h
#     meshobj.h
#     world.h
#     importer.h
# )