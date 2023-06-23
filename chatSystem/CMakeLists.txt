cmake_minimum_required(VERSION 3.15)

project(chatSystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_EXTENSIONS Off)

find_package(Boost 1.71.0 REQUIRED serialization)

file(GLOB_RECURSE SOURCES src/*.cpp)
add_library(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME}
    PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    ${Boost_INCLUDE_DIRS}
    PRIVATE
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    ${Boost_LIBRARIES}
)