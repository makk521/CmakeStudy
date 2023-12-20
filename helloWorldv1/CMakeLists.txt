cmake_minimum_required(VERSION 2.8)  # 指定最低cmake版本

project(helloWorld1) # 设置project的名称，后面PROJECT_NAME就会被改成这个

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -g -O0") # 设置cmake的FLAGS信息

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include) # 设置头文件的搜索路径

FILE(GLOB SOURCE_FILES *.hpp *.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/function.cpp
        # ${CMAKE_CURRENT_SOURCE_DIR}/../hal/*.c
        )

add_executable(${PROJECT_NAME} ${SOURCE_FILES})