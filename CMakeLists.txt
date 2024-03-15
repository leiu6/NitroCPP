cmake_minimum_required(VERSION 3.6)

set(CMAKE_CXX_STANDARD 17)

if(MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Od /std:c++17 /Wall")
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror -fsanitize=address,undefined -std=c++17")
endif()

set(SOURCES src/nitro.cpp
	src/Lexer/Lexer.cpp
	src/AST/ASTNode.cpp
	src/AST/ASTPrettyPrinter.cpp
	src/Parser/Parser.cpp
)

add_executable(nitro ${SOURCES})
