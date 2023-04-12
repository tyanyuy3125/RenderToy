add_library(RenderToy 
            rtmath.cpp
            object.cpp
            importer.cpp
            world.cpp
            renderer.cpp
            exporter.cpp
            procedural.cpp
            ray.cpp
            bvh.cpp
            material.cpp
            surfacepoint.cpp 
            compositor.cpp
            exception.cpp)

set_property(TARGET RenderToy PROPERTY CXX_STANDARD 20)
target_compile_features(RenderToy PRIVATE cxx_std_20)

target_include_directories(RenderToy PRIVATE ${PROJECT_SOURCE_DIR}/include)

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(RenderToy PRIVATE OpenMP::OpenMP_CXX)
endif()
target_compile_options(RenderToy PRIVATE "-march=native")
