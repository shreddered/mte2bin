cmake_minimum_required(VERSION 3.18)

project(asvt)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(asvt
  ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/mtemu.cpp
)

target_include_directories(asvt
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
)
