add_executable(ProceduralTexture main.cpp)
target_link_libraries(ProceduralTexture PRIVATE RenderToy)
target_include_directories(ProceduralTexture PRIVATE ${PROJECT_SOURCE_DIR}/include)
