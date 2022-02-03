file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
add_library(spa ${srcs} ${headers} src/components/query_subsystem/pql/PqlLexer.cpp src/components/query_subsystem/pql/PqlLexer.h src/components/query_subsystem/pql/PqlToken.cpp src/components/query_subsystem/pql/PqlToken.h src/components/source_subsystem/SourceController.cpp src/components/source_subsystem/SourceController.h src/components/query_subsystem/QueryController.cpp src/components/query_subsystem/QueryController.h src/utils/FileUtil.cpp src/utils/FileUtil.h)

# this makes the headers accessible for other projects which uses spa lib
target_include_directories(spa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)


