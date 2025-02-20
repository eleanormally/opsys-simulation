cmake_minimum_required (VERSION 3.5)
project(cpu_simulation)
set(executable simulation)
set (CMAKE_EXPORT_COMPILE_COMMANDS ON)

#including source .cpp files
set(PROJECT_FILES
  ${PROJECT_SOURCE_DIR}/main.cpp
  ${PROJECT_SOURCE_DIR}/arg_parser.cpp
)
add_executable(${executable}
  ${PROJECT_FILES}
)

#preparing gcc
set_target_properties(${executable} PROPERTIES CXX_STANDARD 11)
set_target_properties(${executable} PROPERTIES COMPILE_FLAGS "-g -Wall -Wextra")
