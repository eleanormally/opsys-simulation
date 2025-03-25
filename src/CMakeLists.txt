cmake_minimum_required (VERSION 3.5)
project(cpu_simulation)
set(executable simulation)
set (CMAKE_EXPORT_COMPILE_COMMANDS ON)

#including source .cpp files
set(PROJECT_FILES
  ${PROJECT_SOURCE_DIR}/main.cpp
  ${PROJECT_SOURCE_DIR}/arg_parser.cpp
  ${PROJECT_SOURCE_DIR}/simulation/simulation.cpp
  ${PROJECT_SOURCE_DIR}/simulation/events.cpp
  ${PROJECT_SOURCE_DIR}/time/time.cpp
  ${PROJECT_SOURCE_DIR}/random/random.cpp
  ${PROJECT_SOURCE_DIR}/queue/ready_queue.cpp
  ${PROJECT_SOURCE_DIR}/process/process.cpp
  ${PROJECT_SOURCE_DIR}/process/generate_processes.cpp
)
add_executable(${executable}
  ${PROJECT_FILES}
)

#preparing gcc
set_target_properties(${executable} PROPERTIES CXX_STANDARD 11)
set_target_properties(${executable} PROPERTIES COMPILE_FLAGS "-g -Wall -Wextra -Wpedantic -Werror")
