cmake_minimum_required(VERSION 2.8)  # 指定最低cmake版本

project(helloWorldv2) # 设置project的名称，后面PROJECT_NAME就会被改成这个

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -g -O0") # 设置cmake的FLAGS信息

include_directories(
        "/usr/local/include/eigen3"
        "${CMAKE_SOURCE_DIR}/include/xiong_algorithm"
        "${CMAKE_SOURCE_DIR}/include/commander"
        "${CMAKE_SOURCE_DIR}/src/"
        "${CMAKE_SOURCE_DIR}/include/curlPost"
        "${CMAKE_CURRENT_SOURCE_DIR}"  # 将当前源文件目录添加到头文件搜索路径中
)

file(GLOB_RECURSE SRC_FILES 
        ${CMAKE_SOURCE_DIR}/include/*.cpp
        ${CMAKE_SOURCE_DIR}/src/*.cpp)

# 设置可执行文件的输出路径,PROJECT_SOURCE_DIR表示CMakeLists.txt位置
#                        PROJECT_BINARY_DIR表示cmake编译的目录，即build目录(在哪里执行就是哪里)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_HOME_DIRECTORY}/build/bin) 

add_executable(${PROJECT_NAME} ${SRC_FILES})
target_link_libraries(${PROJECT_NAME} curl)  # 链接 libcurl 动态库 动态库在最后，若自己的动态库还需指定位置
