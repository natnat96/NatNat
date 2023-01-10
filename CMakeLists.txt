cmake_minimum_required(VERSION 3.16)

project(sdl-example)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(MSVC)
    add_compile_options(
        $<$<CONFIG:>:/MT> 
        $<$<CONFIG:Debug>:/MTd>
        $<$<CONFIG:Release>:/MT>
    )
endif()


include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

set(target hellosdl)

add_executable(${target} 
${PROJECT_SOURCE_DIR}/main.cpp
)

target_link_libraries(${target} ${CONAN_LIBS})