cmake_minimum_required(VERSION 3.3)
project(Raph C CXX)

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)
find_package(Boost REQUIRED)

set(BISION_COMPILE_FLAGS "")
list(APPEND BISION_COMPILE_FLAGS --language=C++)
list(APPEND BISION_COMPILE_FLAGS --defines=gen/parser.hpp)
if (CMAKE_BUILD_TYPE MATCHES "Debug")
	list(APPEND BISION_COMPILE_FLAGS --warnings=all)
	list(APPEND BISION_COMPILE_FLAGS --report=all)
	list(APPEND BISION_COMPILE_FLAGS --report-file=gen/parser.txt)
else()
	list(APPEND BISION_COMPILE_FLAGS --warnings=none)
	list(APPEND BISION_COMPILE_FLAGS --no-lines)
endif()
string(REPLACE ";" " " BISION_COMPILE_FLAGS " ${BISION_COMPILE_FLAGS}")

bison_target(Parser parser.y ${CMAKE_SOURCE_DIR}/gen/parser.cpp COMPILE_FLAGS ${BISION_COMPILE_FLAGS})

set(FLEX_COMPILE_FLAGS "")
if (CMAKE_BUILD_TYPE MATCHES "Debug")
	list(APPEND FLEX_COMPILE_FLAGS --debug)
	list(APPEND FLEX_COMPILE_FLAGS --verbose)
else()
	list(APPEND FLEX_COMPILE_FLAGS --noline)
endif()
string(REPLACE ";" " " FLEX_COMPILE_FLAGS " ${FLEX_COMPILE_FLAGS}")
flex_target(Lexer lexer.l ${CMAKE_SOURCE_DIR}/gen/lexer.cpp COMPILE_FLAGS ${FLEX_COMPILE_FLAGS})

add_flex_bison_dependency(Lexer Parser)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp raph.cpp ast.cpp ${FLEX_Lexer_OUTPUTS} ${BISON_Parser_OUTPUTS})
add_executable(Raph ${SOURCE_FILES})
