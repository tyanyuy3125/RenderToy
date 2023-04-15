add_executable(Emissive main.cpp)
target_link_libraries(Emissive PRIVATE RenderToy)
target_include_directories(Emissive PRIVATE ${PROJECT_SOURCE_DIR}/include)
# target_link_directories(Emissive PRIVATE ${PROJECT_SOURCE_DIR}/lib)

# target_link_libraries(Emissive PRIVATE RenderToy OpenImageDenoise)
target_link_directories(Emissive PRIVATE ${PROJECT_SOURCE_DIR}/lib)
target_link_libraries(Emissive PRIVATE "${PROJECT_SOURCE_DIR}/lib/libOpenImageDenoise.so.1")
target_link_libraries(Emissive PRIVATE "${PROJECT_SOURCE_DIR}/lib/libOpenImageDenoise.so.1.4.3")
target_link_libraries(Emissive PRIVATE "${PROJECT_SOURCE_DIR}/lib/libtbb.so.12")
target_link_libraries(Emissive PRIVATE "${PROJECT_SOURCE_DIR}/lib/libtbb.so.12.5")

configure_file(emissive.obj ${EXECUTABLE_OUTPUT_PATH} emissive.obj COPYONLY)
configure_file(room.obj ${EXECUTABLE_OUTPUT_PATH} room.obj COPYONLY)
