add_executable(Drawing main.cpp)
target_link_libraries(Drawing PRIVATE RenderToy)
target_include_directories(Drawing PRIVATE ${PROJECT_SOURCE_DIR}/include)
