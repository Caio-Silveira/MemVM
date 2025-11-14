cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

project(MemVM CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Source files
set(MEMVM_SOURCES
    src/IO/Mmf.cpp
    src/Vmi/Vmi.cpp
)

# Header files (for IDE)
set(MEMVM_HEADERS
    include/IO/Mmf.h
    include/IO/Mmf_Manage.h
    include/IO/Mmf_Read.h
    include/IO/Mmf_Types.h
    include/IO/Mmf_Utils.h
    include/IO/Mmf_Write.h
    include/Vmi/Vmi.h
    include/Vmi/Vmi_Hook.h
    include/Vmi/Vmi_Manage.h
    include/Vmi/Vmi_Process.h
    include/Vmi/Vmi_Types.h
)

add_library(MemVM SHARED 
    main.cpp
    ${MEMVM_SOURCES}
    ${MEMVM_HEADERS}
)

target_include_directories(MemVM PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/minhook/include
)

target_link_libraries(MemVM PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/minhook/lib/libMinHook.a
)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(MemVM PRIVATE
        -Wall
        -Wextra
        -O2
        -fno-exceptions
        -fno-rtti
        -fno-stack-protector
        -ffunction-sections
        -fdata-sections
        -ffreestanding              # CRT min
    )

    target_link_options(MemVM PRIVATE
        -static-libgcc
        -static-libstdc++
        -Wl,--gc-sections
        -Wl,--strip-all
        -Wl,--entry=DllMain
        -s
    )

    target_link_libraries(MemVM PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/minhook/lib/libMinHook.a
        mingw32 
        gcc                         # runtime min
        kernel32
    )
endif()


set_target_properties(MemVM PROPERTIES
    OUTPUT_NAME "MemVM"
    PREFIX ""
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)
