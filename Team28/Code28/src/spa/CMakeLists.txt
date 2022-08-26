file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
add_library(spa ${srcs} ${headers})
# this makes the headers accessible for other projects which uses spa lib
target_include_directories(spa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_executable(create_SolvableQuery "src/QPS/create_SolvableQuery.cpp" "src/QPS/SolvableQuery.cpp" "src/QPS/Declaration.cpp" "src/QPS/SelectClause.cpp" "src/QPS/SuchThatClause.cpp" "src/QPS/PatternClause.cpp" "src/QPS/Synonym.cpp" "src/QPS/RelationshipReference.cpp")