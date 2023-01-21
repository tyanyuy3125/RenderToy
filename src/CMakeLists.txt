#
# CMakeLists.txt for ./src
# File created on 2023/1/10
# Last edited on 2023/1/21
# Tianyu Huang <tianyu@illumiart.net>
#

add_library(OpenPT 
            mathfunc.cpp mathfunc.h 
            meshobj.cpp meshobj.h
            importer.cpp importer.h 
            world.cpp world.h 
            geoobj.h geoobj.cpp
            renderer.h renderer.cpp
            exporter.h exporter.cpp
            camera.h camera.cpp
            procedural.h procedural.cpp
            ray.h ray.cpp
            bvh.h bvh.cpp)

set_property(TARGET OpenPT PROPERTY CXX_STANDARD 17)
