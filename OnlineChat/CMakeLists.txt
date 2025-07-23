cmake_minimum_required(VERSION 3.15)
project(OnlineChat)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Settings for Windows
if(WIN32)
    add_definitions(-D_WIN32_WINNT=0x0601)
endif()

# vcpkg
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)  # if glad - vcpkg
find_package(imgui CONFIG REQUIRED) # if imgui - vcpkg

# Main
add_executable(OnlineChat
    src/main.cpp
    src/BaseInit.cpp
    src/Interface.cpp
    src/Connection.cpp
)

target_include_directories(OnlineChat PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(OnlineChat PRIVATE
    glfw
    glad::glad
    imgui::imgui
    $<$<PLATFORM_ID:Windows>:ws2_32>
    $<$<PLATFORM_ID:Linux>:pthread>
    OpenGL::GL
)

# Auto scaning DLL (only for Windows)
if(WIN32)
    add_custom_command(TARGET OnlineChat POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:glfw>"
        "$<TARGET_FILE_DIR:OnlineChat>"
    )
endif()