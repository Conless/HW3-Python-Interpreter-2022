set(PROJECT_NAME ${CMAKE_PROJECT_NAME})

aux_source_directory(. SRC_LIST)

add_executable(code ${SRC_LIST})

target_link_libraries(code antlr4-cpp-runtime python3-runtime)

add_library(${PROJECT_NAME}_lib STATIC ${SRC_LIST})

add_definitions("-Wall -g")
