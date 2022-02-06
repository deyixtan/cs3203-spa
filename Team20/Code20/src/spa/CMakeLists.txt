file(GLOB_RECURSE srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
add_library(spa ${srcs} ${headers} src/components/query_subsystem/pql_parser/query_validator.cpp src/components/query_subsystem/pql_parser/query_validator.h src/components/query_subsystem/pql_parser/query_grammar_error.cpp src/components/query_subsystem/pql_parser/query_grammar_error.h src/components/pkb/hash.h src/components/query_subsystem/pql_evaluator/query_evaluator.cpp src/components/query_subsystem/pql_evaluator/query_evaluator.h)

# this makes the headers accessible for other projects which uses spa lib
target_include_directories(spa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)


