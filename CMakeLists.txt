cmake_minimum_required(VERSION 3.20)
project(cairo_demo C)

set(CMAKE_C_STANDARD 11)

add_executable(demo WIN32 demo.c)

# static CRT
set_property(TARGET demo PROPERTY
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
)

target_include_directories(demo PRIVATE
    ${CMAKE_SOURCE_DIR}/prefix/include
)

target_link_directories(demo PRIVATE
    ${CMAKE_SOURCE_DIR}/prefix/lib
)

target_link_libraries(demo
    cairo
    freetype
    pixman-1
    fontconfig
    expat
    png16
    zlib
    gdi32
    user32
)
