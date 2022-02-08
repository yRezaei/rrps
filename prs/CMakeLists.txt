#file(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
add_library(prs INTERFACE)
target_include_directories(prs INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)