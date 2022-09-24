cmake_minimum_required(VERSION 3.18)

project(mte2bin)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(mte2bin
  ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/mtemu.cpp
)

target_include_directories(mte2bin
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
)
