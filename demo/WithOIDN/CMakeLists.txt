add_executable(WithOIDN main.cpp)

target_link_directories(WithOIDN PRIVATE ${PROJECT_SOURCE_DIR}/lib)
target_link_libraries(WithOIDN PRIVATE "${PROJECT_SOURCE_DIR}/lib/libOpenImageDenoise.so.1")
target_link_libraries(WithOIDN PRIVATE "${PROJECT_SOURCE_DIR}/lib/libOpenImageDenoise.so.1.4.3")
target_link_libraries(WithOIDN PRIVATE "${PROJECT_SOURCE_DIR}/lib/libtbb.so.12")
target_link_libraries(WithOIDN PRIVATE "${PROJECT_SOURCE_DIR}/lib/libtbb.so.12.5")
target_include_directories(WithOIDN PRIVATE ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(WithOIDN PRIVATE RenderToy)

configure_file(monkey.obj ${EXECUTABLE_OUTPUT_PATH} monkey.obj COPYONLY)
