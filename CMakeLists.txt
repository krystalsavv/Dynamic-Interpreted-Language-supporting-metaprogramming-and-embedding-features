cmake_minimum_required(VERSION 3.4)

project (Interpreter)

set(ROOT ${CMAKE_SOURCE_DIR})
set(PARSER_OUTPUT ${CMAKE_SOURCE_DIR}/src/parser/generated)
file(MAKE_DIRECTORY ${PARSER_OUTPUT})


if(WIN32)
    set(CMAKE_CXX_FLAGS "/EHsc /W3")
    set(CMAKE_CXX_FLAGS_DEBUG "/Zi /Od")
    set(CMAKE_CXX_FLAGS_RELEASE "/O2")
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
elseif(UNIX)
    set(CMAKE_CXX_FLAGS "-Wall -pedantic -g")
    set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g3")
    set(CMAKE_CXX_FLAGS_RELEASE "-O2")
endif()

enable_testing()

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

include_directories(include)
include_directories(${ROOT}/src/parser/generated)

add_subdirectory(src)
add_subdirectory(test/embedding)

