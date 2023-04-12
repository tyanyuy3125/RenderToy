add_executable(Emissive main.cpp)
target_link_libraries(Emissive PRIVATE RenderToy)
target_include_directories(Emissive PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_directories(Emissive PRIVATE ${PROJECT_SOURCE_DIR}/lib)

target_link_libraries(Emissive PRIVATE RenderToy OpenImageDenoise)

configure_file(emissive.obj ${EXECUTABLE_OUTPUT_PATH} emissive.obj COPYONLY)
configure_file(room.obj ${EXECUTABLE_OUTPUT_PATH} room.obj COPYONLY)
