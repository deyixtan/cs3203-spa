file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
add_library(spa ${srcs} ${headers} src/components/query_subsystem/pql_lexer/pql_lexer.cpp src/components/query_subsystem/pql_lexer/pql_lexer.h src/components/query_subsystem/pql_lexer/pql_token.cpp src/components/query_subsystem/pql_lexer/pql_token.h src/components/source_subsystem/SourceController.cpp src/components/source_subsystem/SourceController.h src/components/query_subsystem/query_controller.cpp src/components/query_subsystem/query_controller.h src/utils/FileUtil.cpp src/utils/FileUtil.h src/components/source_subsystem/source_token.cpp src/components/source_subsystem/source_token.h src/components/source_subsystem/source_lexer.cpp src/components/source_subsystem/source_lexer.h src/components/query_subsystem/pql_parser/parsed_query.cpp src/components/query_subsystem/pql_parser/parsed_query.h src/components/query_subsystem/pql_parser/relationship.cpp src/components/query_subsystem/pql_parser/relationship.h src/components/query_subsystem/pql_parser/declaration.cpp src/components/query_subsystem/pql_parser/declaration.h src/components/query_subsystem/pql_parser/pattern.cpp src/components/query_subsystem/pql_parser/pattern.h)

# this makes the headers accessible for other projects which uses spa lib
target_include_directories(spa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)


