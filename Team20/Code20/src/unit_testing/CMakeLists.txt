file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
add_executable(unit_testing ${srcs} src/pql/test_pql_lexer.cpp src/pkb/usage_store_test.cpp src/pkb/usage_store_test.h)

target_link_libraries(unit_testing spa)

