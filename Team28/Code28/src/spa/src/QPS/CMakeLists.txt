file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/Clause/*.cpp" 
"${CMAKE_CURRENT_SOURCE_DIR}/Evaluator/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/Reference/*.cpp")

add_library(qps ${srcs})

# this makes the headers accessible for other projects which uses spa lib
target_include_directories(qps PUBLIC include)