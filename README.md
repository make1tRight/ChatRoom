# ChatRoom
Instant_Messaging_Project in Linux

## 环境依赖项
```bash
# boost
sudo apt-get install libboost-dev libboost-test-dev libboost-all-dev

# jsoncpp
sudo apt install libjsoncpp-dev

# hiredis
sudo apt-get install libhiredis-dev

# mysql
sudo apt install  libmysqlcppconn-dev

# protobuf & grpc
# https://blog.csdn.net/weixin_36378508/article/details/130600632
```

## 登录及聊天界面展示

![login](./login.png)

![chatlogin](./chatlogin.png)

## 分布式服务端

GateServer - 网络服务

StatusServer - 状态服务

ChatServer - 通讯服务

ChatServer2 - 通讯服务2

![chatlogin](./distributed_servers.png)

![communication](./communication.png)
