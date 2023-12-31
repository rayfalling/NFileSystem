cmake_minimum_required(VERSION 3.17)

message("Detect Platform: ${CMAKE_HOST_SYSTEM_PROCESSOR}")

if (MSVC)
    # set character set unicode to make TCHAR is alias of wchar_t
    add_definitions(-DUNICODE -D_UNICODE)
    if (BUILD_SHARED_LIBS)
        set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
    endif (BUILD_SHARED_LIBS)

    add_compile_definitions(MSVC)
    add_compile_definitions(OVERRIDE_PLATFORM_HEADER_NAME=Windows)

    # We dont care C4251(Export std members)
    add_compile_options(/MP /wd4819 /wd5045 /wd4711 /wd4251)
    set(CMAKE_C_FLAGS_DEBUG "/ZI /Ob0 /Od /RTC1")
    set(CMAKE_CXX_FLAGS_DEBUG "/ZI /Ob0 /Od /RTC1")
elseif (not MSVC)
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g")
endif (MSVC)

# judge platform
if (CMAKE_CL_64)
    set(TARGET_ARCH x64)
    message(STATUS "Set Platform x64")
else (CMAKE_CL_64)
    set(TARGET_ARCH x86)
    message(STATUS "Set Platform x86")
endif (CMAKE_CL_64)
