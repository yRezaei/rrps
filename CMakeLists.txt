cmake_minimum_required(VERSION 3.13)
project(multi_threaded_component_system VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

add_subdirectory(rrps)
add_subdirectory(mtcs)

enable_testing()
add_executable(tester test/tester.cpp)
target_link_libraries(tester PUBLIC mtcs rrps)
add_test(Tester tester)