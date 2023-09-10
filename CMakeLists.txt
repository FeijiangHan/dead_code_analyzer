cmake_minimum_required(VERSION 3.7)
project(deadcode)
set (CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES 
    "src/main.cpp"
    src/directory_iterator.h src/directory_iterator.cpp
    src/file_reader.h src/file_reader.cpp
    src/reader.h src/reader.cpp
    src/record_file_reader.h src/record_file_reader.cpp
    src/memref.h src/memtrace_stream.h src/utils.h
    src/trace_entry.h src/trace_entry.cpp
    src/analyzer.h src/analyzer.cpp
    src/analyzer_runner.h src/analyzer_runner.cpp 
    src/analysis_tool.h 
    src/dead_code_analysis.h  src/dead_code_analysis.cpp
)

find_package (Threads)
add_executable(deadcode ${SOURCE_FILES})
target_link_libraries (deadcode Threads::Threads)
