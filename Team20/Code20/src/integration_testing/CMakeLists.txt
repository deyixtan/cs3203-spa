file(GLOB RECURSE "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB RECURSE "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
add_executable(integration_testing ${srcs} src/test_pql_parser_with_lexer.cpp)

target_link_libraries(integration_testing spa)
