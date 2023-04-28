cmake_minimum_required(VERSION 3.1)
project(GuruxDLMS VERSION 1.0.0)
find_package(Threads REQUIRED)
include_directories(${CMAKE_CURRENT_LIST_DIR}/../include/ ${CMAKE_CURRENT_LIST_DIR}/../../development/include/)

link_directories(${CMAKE_CURRENT_LIST_DIR}/../../development/)

file(GLOB Server_SRC
     "*.c"
     "*.cpp"
)

add_executable(GuruxDLMSServer ${Server_SRC})
target_link_libraries(GuruxDLMSServer gurux_dlms_cpp pthread)