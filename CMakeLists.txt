cmake_minimum_required(VERSION 3.0)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 11)

project(rshell)

add_executable(rshell 
               src/main.cpp
               src/rshell.cpp
               src/Lexer.cpp
               src/Parser.cpp
               src/Do.cpp
               src/Token.cpp
               src/SyntaxExcept.cpp)

target_compile_options(rshell PRIVATE -Wall )
               
install(TARGETS rshell RUNTIME DESTINATION bin)



ADD_SUBDIRECTORY(googletest)


ADD_EXECUTABLE(tester  
               src/rshell_test.cpp
               src/Lexer.cpp
               src/Parser.cpp
               src/Do.cpp
               src/Token.cpp
               src/SyntaxExcept.cpp)


TARGET_LINK_LIBRARIES(tester gtest)
