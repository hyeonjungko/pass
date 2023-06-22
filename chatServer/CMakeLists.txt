cmake_minimum_required(VERSION 3.15)

project(chatServer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_EXTENSIONS Off)

add_executable(${PROJECT_NAME} main.cpp)

target_include_directories(${PROJECT_NAME}
    PUBLIC
    chatSystem
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
    chatSystem
)