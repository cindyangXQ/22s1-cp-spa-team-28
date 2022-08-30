file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
add_library(spa ${srcs} ${headers})
add_subdirectory(src/qps)
# this makes the headers accessible for other projects which uses spa lib
target_include_directories(spa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
add_executable(main src/QPS/main.cpp src/QPS/QueryParser.cpp src/QPS/Utils.cpp src/QPS/Clause/Declaration.cpp src/QPS/Clause/SelectClause.cpp src/QPS/Clause/SuchThatClause.cpp src/QPS/Clause/PatternClause.cpp src/QPS/Reference/Synonym.cpp src/QPS/Reference/StatementReference.cpp src/QPS/Reference/EntityReference.cpp)
target_link_libraries(spa qps)
# add_executable(create_SolvableQuery "src/QPS/create_SolvableQuery.cpp" "src/QPS/SolvableQuery.cpp" "src/QPS/Clause/Declaration.cpp" "src/QPS/Clause/SelectClause.cpp" "src/QPS/Clause/SuchThatClause.cpp" "src/QPS/Clause/PatternClause.cpp" "src/QPS/Reference/Synonym.cpp")