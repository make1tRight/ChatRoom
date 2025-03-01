cmake_minimum_required(VERSION 3.12)
project(GateServer)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)

# 设置 Boost 的路径
set(BOOST_ROOT /usr)

# 查找 Boost 库的组件
find_package(Boost REQUIRED COMPONENTS system thread)

# 添加可执行文件和源文件
file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/GateServer/*.cpp)

add_executable(GateServer ${SOURCES})

# 包含头文件路径（包括其他目录）
target_include_directories(GateServer 
    PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/GateServer
)

# 包含 Boost 头文件路径
target_include_directories(GateServer
    PRIVATE 
    ${Boost_INCLUDE_DIRS}
)

# 链接 Boost 库
target_link_libraries(
    GateServer 
    PRIVATE 
    ${Boost_LIBRARIES}
)