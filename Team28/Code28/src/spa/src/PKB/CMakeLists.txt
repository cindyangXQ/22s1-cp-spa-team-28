file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/Facades/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/Tables/*.cpp")

add_library(pkb ${srcs})

# this makes the headers accessible for other projects which uses spa lib
target_include_directories(pkb PUBLIC include)
