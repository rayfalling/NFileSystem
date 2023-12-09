cmake_minimum_required(VERSION 3.18)
project(NFileSystem)

set(CMAKE_CXX_STANDARD 17)

# include cmake files
include(GenerateExportHeader)

# include custom cmake files
include(Cmake/Shared.cmake)
include(Cmake/DetectPlatform.cmake)

# Set Cmake build output
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/Binary/Library)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/Binary)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/Binary)

# Set Include target
set("NFILESYSTEM_INCLUDE_DIR" ${CMAKE_CURRENT_SOURCE_DIR}/Source/Public)
include_directories(${NFILESYSTEM_INCLUDE_DIR}/)
file(GLOB_RECURSE NFILESYSTEM_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/Source/Public/*.h)

# Add source files
aux_source_directory(Source/Public NFILESYSTEM_SOURCE)
aux_source_directory(Source/Private NFILESYSTEM_SOURCE)

# Set source group for VisualStudio
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${NFILESYSTEM_INCLUDE} ${NFILESYSTEM_SOURCE})

# Add the library
add_library(${PROJECT_NAME} ${LIB_TYPE} ${NFILESYSTEM_INCLUDE} ${NFILESYSTEM_SOURCE})

# Set shared export marco file
if (BUILD_SHARED_LIBS)
    target_compile_definitions(${PROJECT_NAME} PRIVATE NFILESYSTEM_SHARED)
    GENERATE_EXPORT_HEADER(
            ${PROJECT_NAME}
            BASE_NAME NFILESYSTEM
            EXPORT_MACRO_NAME NFILESYSTEM_EXPORTS
            EXPORT_FILE_NAME ${NFILESYSTEM_INCLUDE_DIR}/NFileSystemExport.generate.h
            STATIC_DEFINE NFILESYSTEM_EXPORTS_BUILT_AS_STATIC
    )
endif (BUILD_SHARED_LIBS)

# Set target properties
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER NFILESYSTEM)

# Set target library export headers
target_include_directories(${PROJECT_NAME} PUBLIC ${NFILESYSTEM_INCLUDE_DIR})

#target_link_libraries(${PROJECT_NAME})
