cmake_minimum_required(VERSION 3.14)

project(xcb_window)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")

add_executable(xcb_window 
    main.cpp
    Window.hpp
    Window.cpp
)

target_link_libraries(xcb_window PUBLIC xcb)
