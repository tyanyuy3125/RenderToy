#
# CMakeLists.txt for ./src
# File created on 2023/1/10
# Last edited on 2023/1/21
# Tianyu Huang <tianyu@illumiart.net>
#

add_library(RenderToy 
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
            bvh.h bvh.cpp
            material.h material.cpp
            random.h random.cpp
            surfacepoint.h surfacepoint.cpp)

set_property(TARGET RenderToy PROPERTY CXX_STANDARD 17)

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(RenderToy PUBLIC OpenMP::OpenMP_CXX)
endif()
target_compile_options(RenderToy PUBLIC "-march=native")
# target_compile_options(RenderToy PUBLIC "-DENABLE_CULLING")
# target_compile_options(RenderToy PUBLIC "-DDISABLE_BVH")
set_directory_properties(PROPERTIES COMPILE_DEFINITIONS_DEBUG "_DEBUG")
