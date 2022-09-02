file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/Clause/*.cpp" 
"${CMAKE_CURRENT_SOURCE_DIR}/Evaluator/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/Reference/*.cpp")
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")

add_library(sp ${srcs})

# this makes the headers accessible for other projects which uses spa lib
target_include_directories(sp PUBLIC include)

