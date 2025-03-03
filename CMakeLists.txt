cmake_minimum_required(VERSION 3.12)
project(GateServer)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)

# 设置 Boost 的路径
set(BOOST_ROOT /usr)
set(gRPC_ROOT /usr/local/lib/cmake/grpc)
set(absl_ROOT /usr/local/lib/cmake/absl)

# set(Protobuf_ROOT /usr/local)
# set(CMAKE_PREFIX_PATH "/usr/local")

set(Protobuf_PROTOC_EXECUTABLE "/usr/local/bin/protoc" CACHE FILEPATH "Path to protoc compiler")
set(Protobuf_INCLUDE_DIR "/usr/local/include")
set(Protobuf_LIBRARY "/usr/local/lib/libprotobuf.a")
set(Protobuf_LITE_LIBRARY "/usr/local/lib/libprotobuf-lite.a")
set(Protobuf_PROTOC_LIBRARY "/usr/local/lib/libprotoc.a")


# 查找 Boost 库的组件
find_package(Boost REQUIRED COMPONENTS filesystem thread)
find_package(Protobuf HINTS ${Protobuf_ROOT} CONFIG REQUIRED)
# find_package(Protobuf REQUIRED)
find_package(absl HINTS ${absl_ROOT} CONFIG REQUIRED)
find_package(jsoncpp REQUIRED)
find_package(gRPC REQUIRED PATHS ${gRPC_ROOT} NO_DEFAULT_PATH)

find_library(HIREDIS_LIB hiredis)

# 添加可执行文件和源文件
file(GLOB
    SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/GateServer/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GateServer/*.cc
)

add_executable(GateServer ${SOURCES})

# 包含头文件路径（包括其他目录）
target_include_directories(GateServer 
    PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/GateServer
    ${gRPC_ROOT}/include
    ${Protobuf_INCLUDE_DIRS}
)

# 链接相关库
target_link_libraries(
    GateServer 
    PRIVATE
    ${Boost_LIBRARIES}
    ${HIREDIS_LIB}
    ${gRPC_LIBRARIES}
    ${Protobuf_LIBRARY}
    ${Protobuf_LITE_LIBRARY}
    ${Protobuf_PROTOC_LIBRARY}
    absl::base
    absl::debugging
    absl::flags
    absl::hash
    absl::log
    absl::memory
    absl::meta
    absl::numeric
    absl::raw_logging_internal
    absl::strings
    absl::synchronization
    absl::time
    absl::utility
    absl::variant
    absl::symbolize
    absl::stacktrace
    absl::debugging_internal
    jsoncpp
)
