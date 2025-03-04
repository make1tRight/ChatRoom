cmake_minimum_required(VERSION 3.12)
project(StatusServer)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 20)

# 查找依赖项的组件(include&lib)
find_package(Boost REQUIRED COMPONENTS filesystem thread)
find_package(Protobuf REQUIRED)
find_package(jsoncpp REQUIRED)
find_package(gRPC CONFIG REQUIRED)
find_package(utf8_range REQUIRED)

message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
message(STATUS "Protobuf_INCLUDE_DIRS: ${Protobuf_INCLUDE_DIRS}")
message(STATUS "Protobuf_LIBRARIES: ${Protobuf_LIBRARIES}")


# 添加可执行文件和源文件
file(GLOB
    SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/StatusServer/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/StatusServer/*.cc
)

add_executable(StatusServer ${SOURCES})

# 包含头文件路径（包括其他目录）
target_include_directories(
    StatusServer
    PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/StatusServer
    ${Boost_INCLUDE_DIRS}
    ${Protobuf_INCLUDE_DIRS}
)

# 链接相关库
target_link_libraries(
    StatusServer 
    PRIVATE
    ${Boost_LIBRARIES}
    ${Protobuf_LIBRARIES}
    gRPC::grpc++
    hiredis
    jsoncpp
    mysqlcppconn
    utf8_range::utf8_range
    utf8_range::utf8_validity
)
