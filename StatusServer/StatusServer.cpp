// StatusServer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <jsoncpp/json/json.h>
#include <jsoncpp/json/value.h>
#include <jsoncpp/json/reader.h>
#include "const.h"
#include "ConfigMgr.h"
#include <hiredis/hiredis.h>
#include "RedisMgr.h"
#include "MysqlMgr.h"
#include "AsioIOContextPool.h"
#include <memory>
#include <string>
#include <thread>
#include <boost/asio.hpp>
#include "StatusServiceImpl.h"



void RunServer() {
    auto& cfg = ConfigMgr::Inst();
    std::string server_address(cfg["StatusServer"]["Host"] + ":" + cfg["StatusServer"]["Port"]);
    StatusServiceImpl service;
    grpc::ServerBuilder builder;
    //�����˿ں����ӷ���
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    //����������gRPC������
    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;
    //����Boost.Asio��io_context
    boost::asio::io_context io_context;
    //����signal_set���ڲ���SIGINT
    boost::asio::signal_set signals(io_context, SIGINT, SIGTERM);
    //�����첽�ȴ�SIGINT�ź�
    signals.async_wait([&server, &io_context](const boost::system::error_code& error, int signal_number) {
        if (!error) {
            std::cout << "Shutting down server..." << std::endl;
            server->Shutdown();
            io_context.stop();
        }
    });
    //�ڵ����߳�������io_context
    std::thread([&io_context]() { io_context.run(); }).detach();//[17-46:52]Ϊʲô����Ҫ����������һ���߳�
    //�ȴ��������ر�
    server->Wait();
}

int main()
{
    try {
        RunServer();
    }
    catch (std::exception const& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return 0;
}
