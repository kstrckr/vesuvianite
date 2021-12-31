cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 20)

project(Vesuvianite VERSION 0.1.0 LANGUAGES CXX)

add_executable(vesuvianite src/main.cxx)
# add_library(rawLoader src/RawLoader.cxx)
# target_include_directories(rawLoader PRIVATE libraries/RawLoader)

# target_include_directories(mylib PUBLIC include)
# target_include_directories(mylib PRIVATE src)

# target_link_libraries(mylib PUBLIC abc)
# target_link_libraries(mylib PRIVATE xyz)

# find_package(OpenCV)