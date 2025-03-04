# ChatRoom
Instant messaging project in Linux
## 开发背景
1. 提供高效实时的沟通平台
2. 为管理用户账号实现注册/登录/修改密码功能
3. 为传递信息/实时通讯实现聊天功能

## 环境依赖项
```bash
# boost
sudo apt-get install libboost-dev libboost-test-dev libboost-all-dev

# jsoncpp
sudo apt install libjsoncpp-dev

# hiredis
sudo apt-get install libhiredis-dev

# mysql
sudo apt install libmysqlcppconn-dev

# protobuf & grpc protobuf和grpc版本要对应
# 先编译protobuf后编译grpc
# 具体参考下面2链接
# https://blog.csdn.net/weixin_36378508/article/details/130600632
# https://developer.aliyun.com/article/819208
```

## 客户端界面展示
### 登录
![login](./login.png)
### 聊天
![chatlogin](./chatlogin.png)

## 分布式服务端
### GateServer - 网络服务
- 处理http短连接

### StatusServer - 状态服务
- 实现负载均衡

### VarifyServer - 验证服务
- 实现验证码生成及发送逻辑, 验证码可用于账号注册和修改密码

### ChatServer/ChatServer2- 通讯服务
- 与客户端建立tcp长连接
- 实现聊天、添加好友等逻辑

![chatlogin](./distributed_servers.png)

![communication](./communication.png)


## 编译注意事项
### 下载并编译grpc
```bash
# 1. 获取源码包
git clone https://github.com/grpc/grpc.git`
# 2. 更新gpc依赖的第三方库
cd grpc 
git submodule update --init

# 3. 编译
mkdir -p cmake/build
pushd cmake/build
## 编译并安装 | -DgRPC_INSTALL=ON 
## 安装abseil | -DABSL_ENABLE_INSTALL=TRUE 安装abseil
## -DBUILD_SHARED_LIBS=ON | 编译成动态库[去掉再试试]
## CMakeLists.txt所在目录 | ../..
cmake -DgRPC_INSTALL=ON -DABSL_ENABLE_INSTALL=TRUE ../..
make -j 4
# 4. 安装 | 默认位置是/usr/local
sudo make install
popd
```
### 编译grpc包中的protobuf
```bash
# 1. 更新gpc依赖的第三方库
cd third_party/protobuf
git submodule update --init --recursive
# bazel编译protobuf
sudo apt-get install g++ git bazel
bazel build :protoc :protobuf --enable_bzlmod
# 拷贝到工作目录中去
cp bazel-bin/protoc /usr/local/bin

mkdir -p cmake/build
pushd cmake/build
cmake ../..
make -j 4
sudo make install
```

### 先编译protobuf后编译grpc

### 最终加入utf8_range库成功编译了一次
```
find_package(utf8_range REQUIRED)

target_link_libraries(
    GateServer 
    PRIVATE
    utf8_range::utf8_range
    utf8_range::utf8_validity
)

# cmake .. 出现以下内容不影响项目的编译
#CMake Warning at /usr/share/cmake-3.22/Modules/FindProtobuf.cmake:524 (message):
#  Protobuf compiler version 29.0 doesn't match library version 5.29.0
#Call Stack (most recent call first):
#  CMakeLists.txt:13 (find_package)

```
