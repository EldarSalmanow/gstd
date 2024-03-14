cmake_minimum_required(VERSION 3.23)
project(gstd)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(LIBRARY_NAME  ${PROJECT_NAME})
set(PROJECT_DIR   ${CMAKE_SOURCE_DIR})
set(INCLUDE_DIR   ${PROJECT_DIR}/include)
set(SOURCE_DIR    ${PROJECT_DIR}/src)
set(BINARY_DIR    ${CMAKE_BINARY_DIR})

message(STATUS "gstd")
message(STATUS "    LibraryName:  ${LIBRARY_NAME}")
message(STATUS "    ProjectDir:   ${PROJECT_DIR}")
message(STATUS "    IncludeDir:   ${INCLUDE_DIR}")
message(STATUS "    SourceDir:    ${SOURCE_DIR}")
message(STATUS "    BinaryDir:    ${BINARY_DIR}")

find_package(fmt REQUIRED)

add_executable(gstd main.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt)
