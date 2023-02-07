add_executable(WithOIDN main.cpp)

target_include_directories(WithOIDN PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_directories(WithOIDN PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib)

target_link_libraries(WithOIDN PRIVATE RenderToy OpenImageDenoise)

configure_file(monkey.obj ${EXECUTABLE_OUTPUT_PATH} monkey.obj COPYONLY)
