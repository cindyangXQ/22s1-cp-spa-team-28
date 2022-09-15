
file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
add_library(spa ${srcs} ${headers})

add_subdirectory(src/commons)
add_subdirectory(src/pkb)
add_subdirectory(src/qps)
add_subdirectory(src/sp)
# this makes the headers accessible for other projects which uses spa lib
target_include_directories(spa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(spa commons)
target_link_libraries(spa pkb)
target_link_libraries(spa qps)
target_link_libraries(spa sp)
target_compile_options(spa PRIVATE 
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
        -Werror -Wall -Wextra -Wconversion -Wsign-conversion -pedantic-errors>
    $<$<CXX_COMPILER_ID:MSVC>:
        /W4 /WX>)
