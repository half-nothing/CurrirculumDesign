cmake_minimum_required(VERSION 3.24)
project(CurriculumDesign)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXE_LINKER_FLAGS -static)
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -O2")

add_executable(CurriculumDesign main.cpp)
