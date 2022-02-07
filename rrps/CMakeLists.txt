#file(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
add_library(rrps INTERFACE)
target_include_directories(rrps INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)