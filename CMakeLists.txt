### YOU CAN'T MODIFY THE CODE BELOW
cmake_minimum_required(VERSION 3.10.2)
project(python_interpreter)
set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
include_directories(
        ${PROJECT_SOURCE_DIR}/generated/
        ${PROJECT_SOURCE_DIR}/third_party/
        ${PROJECT_SOURCE_DIR}/third_party/runtime/src/
        ${PROJECT_SOURCE_DIR}/include/
        ${PROJECT_SOURCE_DIR}/src/
)
set(src_dir
        ${PROJECT_SOURCE_DIR}/generated/Python3Lexer.cpp
        ${PROJECT_SOURCE_DIR}/generated/Python3Parser.cpp
        ${PROJECT_SOURCE_DIR}/generated/Python3Listener.cpp
        ${PROJECT_SOURCE_DIR}/generated/Python3BaseListener.cpp
        ${PROJECT_SOURCE_DIR}/src/Evalvisitor.cc
        ${PROJECT_SOURCE_DIR}/src/Exception.cc
        ${PROJECT_SOURCE_DIR}/src/main.cpp
        )
file(GLOB antlr4-cpp-src
        ${PROJECT_SOURCE_DIR}/third_party/runtime/src/*.cpp
        ${PROJECT_SOURCE_DIR}/third_party/runtime/src/atn/*.cpp
        ${PROJECT_SOURCE_DIR}/third_party/runtime/src/dfa/*.cpp
        ${PROJECT_SOURCE_DIR}/third_party/runtime/src/misc/*.cpp
        ${PROJECT_SOURCE_DIR}/third_party/runtime/src/support/*.cpp
        ${PROJECT_SOURCE_DIR}/third_party/runtime/src/tree/*.cpp
        ${PROJECT_SOURCE_DIR}/third_party/runtime/src/tree/pattern/*.cpp
        ${PROJECT_SOURCE_DIR}/third_party/runtime/src/tree/xpath/*.cpp
        )
add_library (antlr4-cpp-runtime ${antlr4-cpp-src})
### YOU CAN"T MODIFY THE CODE ABOVE

add_executable(code ${src_dir} src/main.cpp) # Add all *.cpp file after src/main.cpp, like src/Evalvisitor.cpp did

### YOU CAN'T MODIFY THE CODE BELOW
target_link_libraries(code antlr4-cpp-runtime)
### YOU CAN"T MODIFY THE CODE ABOVE
