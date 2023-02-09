add_executable(SmoothShading main.cpp)
target_link_libraries(SmoothShading PRIVATE RenderToy)
configure_file(ss.obj ${EXECUTABLE_OUTPUT_PATH} ss.obj COPYONLY)
