cmake_minimum_required(VERSION 3.22)

project(SweetBread VERSION 1.0 LANGUAGES CXX)

add_subdirectory(Audio)

add_executable(SweetBread
    main.cpp
    imgui.cpp
    imgui.h
    imgui_impl_opengl3.cpp
    imgui_impl_opengl3.h
    imgui_impl_win32.cpp
    imgui_impl_win32.h
    imgui_stdlib.h
    imgui_stdlib.cpp
    imgui_draw.cpp
    imgui_tables.cpp
    imgui_widgets.cpp
)
target_link_libraries(SweetBread 
    PRIVATE 
        Audio
        opengl32
)
set_target_properties(SweetBread PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

# copy assets
add_custom_command(TARGET SweetBread POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:SweetBread>/assets
)