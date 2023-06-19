cmake_minimum_required(VERSION 3.20)

project(bpaint_march)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL ${CMAKE_CURRENT_SOURCE_DIR}/)
set(COMMANDS_FILE ${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json)

if (EXISTS ${COMMANDS_FILE})
  file(COPY ${COMMANDS_FILE} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/)
endif()

add_executable(bpaint_march ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c)

include_directories(./include)

add_dependencies(bpaint_march bpaint_static)
target_link_libraries(bpaint_march PRIVATE bpaint_static)
