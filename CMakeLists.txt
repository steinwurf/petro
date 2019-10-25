cmake_minimum_required (VERSION 3.12)
project(petro)

if(NOT DEFINED STEINWURF_RESOLVE)
    set(STEINWURF_RESOLVE TRUE)
    message(STATUS "Resolving dependencies...")
    execute_process(
        COMMAND python waf resolve
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endif()

file(GLOB_RECURSE petro_sources ./src/*.cpp)
add_library(petro STATIC ${petro_sources})
add_library(steinwurf::petro ALIAS petro)

add_subdirectory("${CMAKE_SOURCE_DIR}/resolve_symlinks/endian" endian)

target_link_libraries(petro
    PRIVATE steinwurf::endian)

target_compile_features(petro PUBLIC cxx_std_14)

target_include_directories(petro INTERFACE src)
