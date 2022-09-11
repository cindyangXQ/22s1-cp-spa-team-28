file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(sp ${srcs})

# this makes the headers accessible for other projects which uses spa lib
target_include_directories(sp PUBLIC include)

