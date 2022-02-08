#file(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
add_library(mtcs INTERFACE)
target_include_directories(mtcs INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(mtcs INTERFACE rrps)