cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

project(bst CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(bst SHARED 
    main.cpp
    src/IO/Mmf.cpp
    src/Vmi/Vmi.cpp
)

target_include_directories(bst PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/minhook/include
)

target_link_libraries(bst PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/minhook/lib/libMinHook.a
)
