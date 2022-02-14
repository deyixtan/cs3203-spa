file(GLOB_RECURSE srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")

add_library(spa ${srcs} ${headers} src/components/query_subsystem/pql_parser/parsed_query_builder.cpp src/components/query_subsystem/pql_parser/parsed_query_builder.h src/components/query_subsystem/pql_evaluator/query_result.cpp src/components/query_subsystem/pql_evaluator/query_result.h src/components/query_subsystem/pql_evaluator/query_condition.cpp src/components/query_subsystem/pql_evaluator/query_condition.h)

# this makes the headers accessible for other projects which uses spa lib
target_include_directories(spa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)


