add_executable(CornellBox main.cpp)
target_link_libraries(CornellBox PRIVATE RenderToy)
configure_file(cornellbox.obj ${EXECUTABLE_OUTPUT_PATH} cornellbox.obj COPYONLY)
