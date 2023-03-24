add_executable(SmoothShading main.cpp)
target_link_libraries(SmoothShading PRIVATE RenderToy)
target_include_directories(SmoothShading PRIVATE ${PROJECT_SOURCE_DIR}/include)
configure_file(ss.obj ${EXECUTABLE_OUTPUT_PATH} ss.obj COPYONLY)
