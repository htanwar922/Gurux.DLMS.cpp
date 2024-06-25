cmake_minimum_required(VERSION 3.5)
if (POLICY CMP0048)
  cmake_policy(SET CMP0048 NEW)
endif (POLICY CMP0048)

project(GuruxDLMS VERSION 1.0.0)

execute_process(COMMAND lsb_release -si OUTPUT_VARIABLE PLATFORM_ID)
message(STATUS "\nPlatform ID : ${PLATFORM_ID}")

# set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_DEBUG -DASIO_STANDALONE -DASIO_HAS_STD_CHRONO")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -fPIC -D_DEBUG -DASIO_STANDALONE -DASIO_HAS_STD_CHRONO")
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  add_compile_options(/W3)
  include_directories(${CMAKE_SOURCE_DIR}/vcpkg_installed/x64-windows/include)
  link_directories(${CMAKE_SOURCE_DIR}/vcpkg_installed/x64-windows/lib)
  link_libraries(ws2_32 pthreadVC3)
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-reorder -Wno-unused-variable")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-sign-compare -Wno-maybe-uninitialized")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-but-set-variable -Wno-parentheses")
  link_libraries(pthread)
endif()

add_definitions(-D_GLIBCXX_USE_CXX11_ABI=0)

add_subdirectory(development)

link_directories(${CMAKE_SOURCE_DIR}/development/)

add_subdirectory(GuruxDLMSServerExample)
add_subdirectory(GuruxDLMSClientExample)
add_subdirectory(GuruxDLMSMeterListener)
add_subdirectory(GuruxDLMSPushExample)

# message(STATUS "Client_SRC : ${Client_SRC}")
# message(STATUS "Server_SRC : ${Server_SRC}")
# message(STATUS "Listener_SRC : ${Listener_SRC}")
# message(STATUS "Pusher_SRC : ${Pusher_SRC}")

add_executable(GuruxDLMSClient ${Client_SRC})
target_link_libraries(GuruxDLMSClient gurux_dlms_cpp)
add_executable(GuruxDLMSServer ${Server_SRC})
target_link_libraries(GuruxDLMSServer gurux_dlms_cpp)
add_executable(GuruxDLMSListener ${Listener_SRC})
target_link_libraries(GuruxDLMSListener gurux_dlms_cpp)
add_executable(GuruxDLMSPusher ${Pusher_SRC})
target_link_libraries(GuruxDLMSPusher gurux_dlms_cpp)

unset(Client_SRC CACHE)
unset(Server_SRC CACHE)
unset(Listener_SRC CACHE)
unset(Pusher_SRC CACHE)