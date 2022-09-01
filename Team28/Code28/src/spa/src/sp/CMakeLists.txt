file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/Clause/*.cpp" 
"${CMAKE_CURRENT_SOURCE_DIR}/Evaluator/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/Reference/*.cpp")

add_library(sp ${srcs} "Node.cpp" "Node.h" "ExprParser.cpp" "ExprParser.h"  "Parser.h")

# this makes the headers accessible for other projects which uses spa lib
target_include_directories(qps PUBLIC include)
