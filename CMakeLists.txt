cmake_minimum_required(VERSION 3.20)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")

project(bpaint)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL ${CMAKE_CURRENT_SOURCE_DIR}/)
set(COMMANDS_FILE ${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json)

if (EXISTS ${COMMANDS_FILE})
  file(COPY ${COMMANDS_FILE} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/)
endif()

#file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/run_tests.sh DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
#file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
file(GLOB bpaint_sources
  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/formats/*.c
)

add_executable(bpaint_e ${bpaint_sources})
add_library(bpaint_shared SHARED ${bpaint_sources})
add_library(bpaint_static STATIC ${bpaint_sources})

# set(BPAINT_COMPILE_FLAGS -g -Wall)
set(BPAINT_COMPILE_FLAGS_DEBUG -g -Wall)
target_compile_options(bpaint_e PRIVATE ${BPAINT_COMPILE_FLAGS_DEBUG})
target_compile_options(bpaint_shared PRIVATE ${BPAINT_COMPILE_FLAGS_DEBUG})
target_compile_options(bpaint_static PRIVATE ${BPAINT_COMPILE_FLAGS_DEBUG})


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

target_include_directories(
  bpaint_e
  PUBLIC
  $<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  ${CMAKE_CURRENT_BINARY_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/include)


target_include_directories(
  bpaint_shared
  PUBLIC
  $<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  ${CMAKE_CURRENT_BINARY_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/include)



target_include_directories(
  bpaint_static
  PUBLIC
  $<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  ${CMAKE_CURRENT_BINARY_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/include)


set(BPAINT_DEPS
  m
  pthread
)
target_link_libraries(bpaint_e PUBLIC ${BPAINT_DEPS})
target_link_libraries(bpaint_shared PUBLIC ${BPAINT_DEPS})
target_link_libraries(bpaint_static PUBLIC ${BPAINT_DEPS})
# add_subdirectory(test)

