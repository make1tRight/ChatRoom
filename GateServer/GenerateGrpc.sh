#!/bin/bash

# 删除旧的 gRPC 生成文件
echo "Removing old message.grpc.pb.* & message.pb.*"
rm -f message.grpc.pb.h message.grpc.pb.cc message.pb.h message.pb.cc

# 设置gRPC插件路径
GRPC_PLUGIN_PATH="/usr/local/bin/grpc_cpp_plugin"
PROTO_FILE="message.proto"

echo "Generating gRPC code..."
protoc -I=. --grpc_out=. --plugin=protoc-gen-grpc="$GRPC_PLUGIN_PATH" "$PROTO_FILE"

echo "Generating CXX code..."
protoc -I=. --cpp_out=. "$PROTO_FILE"

echo "Done."
