cmake_minimum_required(VERSION 3.11)
project(test CXX)
set(CMAKE_CXX_STANDARD 11)


set(JSON_BuildTests OFF CACHE INTERNAL "")
set(JSON_Install OFF CACHE INTERNAL "")
add_subdirectory(nlohmann_json_cmake_fetchcontent)
add_subdirectory(curlpp)
include_directories("${CMAKE_SOURCE_DIR}/curlpp/include")


add_executable(main main.cpp)
target_link_libraries(main PRIVATE nlohmann_json::nlohmann_json curlpp curl)
