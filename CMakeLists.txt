cmake_minimum_required(VERSION 3.10)

project(MassifTool)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#set(CMAKE_BUILD_TYPE Debug)

set(SOURCES
	src/main.cpp
	src/memory_analysis.cpp
	src/parser.cpp
)

set(HEADERS
	src/memory_analysis.h
	src/parser.h
	src/snapshot.h
)

add_executable(massif_tool ${SOURCES} ${HEADERS})