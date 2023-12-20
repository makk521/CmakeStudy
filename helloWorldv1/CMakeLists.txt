cmake_minimum_required(VERSION 2.8)  # 指定最低cmake版本

project(helloWorld1) # 设置project的名称，后面PROJECT_NAME就会被改成这个

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -g -O0") # 设置cmake的FLAGS信息

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include) # 设置头文件的搜索路径

FILE(GLOB SOURCE_FILES *.hpp *.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/function.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
        # ${CMAKE_CURRENT_SOURCE_DIR}/../hal/*.c
        )

# 设置可执行文件的输出路径,PROJECT_SOURCE_DIR表示CMakeLists.txt位置
#                        PROJECT_BINARY_DIR表示cmake编译的目录，即build目录(在哪里执行就是哪里)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/build/bin) 

add_executable(${PROJECT_NAME} ${SOURCE_FILES})