cmake_minimum_required(VERSION 3.17)

option(BUILD_SHARED "Build using shared libraries" ON)

set(LIB_TYPE STATIC)
if (BUILD_SHARED)
    # User wants to build Dynamic Libraries, so change the LIB_TYPE variable to CMake keyword 'SHARED'
    set(LIB_TYPE SHARED)
endif (BUILD_SHARED)
