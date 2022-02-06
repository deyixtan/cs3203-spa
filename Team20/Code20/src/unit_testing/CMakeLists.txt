file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
add_executable(unit_testing ${srcs} src/test_pql_lexer.cpp src/test_query_validator.cpp src/test_query_builder.cpp src/sample_token.cpp src/sample_token.h src/test_util.h src/pkb/follow_store_test.cpp src/pkb/follow_store_stub.h)

target_link_libraries(unit_testing spa)

