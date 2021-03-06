cmake_minimum_required(VERSION 3.10)
project(petro)

find_package(Python COMPONENTS Interpreter)
if(NOT DEFINED STEINWURF_RESOLVE)
  message(STATUS "Resolving dependencies...")
  execute_process(
    COMMAND ${Python_EXECUTABLE} waf resolve ${STEINWURF_RESOLVE_OPTIONS}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    RESULT_VARIABLE STATUS)
  if(STATUS AND NOT STATUS EQUAL 0)
    message(FATAL_ERROR "Failed: ${STATUS}")
  endif()
    set(STEINWURF_RESOLVE "${CMAKE_CURRENT_SOURCE_DIR}/resolve_symlinks")
endif()

file(GLOB_RECURSE petro_sources ./src/*.cpp)
add_library(petro STATIC ${petro_sources})
add_library(steinwurf::petro ALIAS petro)

if (NOT TARGET endian)
  add_subdirectory("${STEINWURF_RESOLVE}/endian" endian)
endif()

if(CMAKE_C_COMPILER_ID MATCHES "MSVC")
file(GLOB_RECURSE stream_cpp ./src/stream.cpp)
set_source_files_properties(${stream_cpp} PROPERTIES COMPILE_DEFINITIONS _CRT_SECURE_NO_WARNINGS)
endif()

target_link_libraries(petro
    PRIVATE steinwurf::endian)

target_compile_features(petro PUBLIC cxx_std_14)

target_include_directories(petro INTERFACE src)

install(DIRECTORY ./src/petro
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include
        FILES_MATCHING PATTERN *.hpp)

install(FILES $<TARGET_FILE:petro> DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
