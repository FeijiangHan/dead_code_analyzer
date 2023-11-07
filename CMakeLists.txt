cmake_minimum_required(VERSION 3.7)
# Project name
project(deadcode)
# Set compilation version
set (CMAKE_CXX_STANDARD 14)
# 1. Header files and cpp files of the project itself
set(SOURCE_FILES
        src/trace/cpps/main.cpp
        src/trace/headers/directory_iterator.h src/trace/cpps/directory_iterator.cpp
        src/trace/headers/file_reader.h src/trace/cpps/file_reader.cpp
        src/trace/headers/reader.h src/trace/cpps/reader.cpp
        src/trace/headers/record_file_reader.h src/trace/headers/record_file_reader.cpp
        src/trace/headers/memref.h src/trace/headers/memtrace_stream.h src/trace/headers/utils.h
        src/trace/headers/trace_entry.h src/trace/cpps/trace_entry.cpp
        src/trace/headers/analyzer.h src/trace/cpps/analyzer.cpp
        src/trace/headers/analyzer_runner.h src/trace/cpps/analyzer_runner.cpp
        src/trace/headers/analysis_tool.h
        src/trace/headers/dead_code_analysis.h src/trace/cpps/dead_code_analysis.cpp)

# Set external library links
set(ZLIB_INCLUDE src/xed/include)
set(ZLIB_LIBRARIES src/xed/lib)

# Link external libraries
include_directories(${ZLIB_INCLUDE})
link_directories(${ZLIB_LIBRARIES})
find_package (Threads)
# Set compilation files
add_executable(deadcode ${SOURCE_FILES})

# Link third-party libraries together
target_link_libraries (deadcode libxed.a Threads::Threads)