cmake_minimum_required(VERSION 3.1)
if (POLICY CMP0048)
  cmake_policy(SET CMP0048 NEW)
endif (POLICY CMP0048)

project(GuruxDLMS VERSION 1.0.0)

# For Raspberry pi
execute_process(COMMAND lsb_release -si OUTPUT_VARIABLE PLATFORM_ID)
message(STATUS "\nPlatform ID : ${PLATFORM_ID}")

# set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -fPIC -D_DEBUG -DASIO_STANDALONE -DASIO_HAS_STD_CHRONO")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_DEBUG -DASIO_STANDALONE -DASIO_HAS_STD_CHRONO")

add_definitions(-D_GLIBCXX_USE_CXX11_ABI=0)

add_subdirectory(development)
add_subdirectory(GuruxDLMSServerExample)
add_subdirectory(GuruxDLMSClientExample)
