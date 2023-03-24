add_executable(CornellBox main.cpp)
target_link_libraries(CornellBox PRIVATE RenderToy)
target_include_directories(CornellBox PRIVATE ${PROJECT_SOURCE_DIR}/include)
configure_file(cornellbox.obj ${EXECUTABLE_OUTPUT_PATH} cornellbox.obj COPYONLY)
