#include <iostream>
#include <jsoncpp/json/json.h>
#include <jsoncpp/json/value.h>
#include <jsoncpp/json/reader.h>
#include "CServer.h"
#include "ConfigMgr.h"
#include "const.h"
#include "RedisMgr.h"


void TestRedis() {
    //����redis ��Ҫ�����ſ��Խ�������
//redisĬ�ϼ����˿�Ϊ6387 �����������ļ����޸�
    redisContext* c = redisConnect("81.68.86.146", 6380);
    if (c->err)
    {
        printf("Connect to redisServer faile:%s\n", c->errstr);
        redisFree(c);        return;
    }
    printf("Connect to redisServer Success\n");

    std::string redis_password = "123456";
    redisReply* r = (redisReply*)redisCommand(c, "AUTH %s", redis_password.c_str());
    if (r->type == REDIS_REPLY_ERROR) {
        printf("Redis��֤ʧ�ܣ�\n");
    }
    else {
        printf("Redis��֤�ɹ���\n");
    }

    //Ϊredis����key
    const char* command1 = "set stest1 value1";

    //ִ��redis������
    r = (redisReply*)redisCommand(c, command1);

    //�������NULL��˵��ִ��ʧ��
    if (NULL == r)
    {
        printf("Execut command1 failure\n");
        redisFree(c);        return;
    }

    //���ִ��ʧ�����ͷ�����
    if (!(r->type == REDIS_REPLY_STATUS && (strcmp(r->str, "OK") == 0 || strcmp(r->str, "ok") == 0)))
    {
        printf("Failed to execute command[%s]\n", command1);
        freeReplyObject(r);
        redisFree(c);        return;
    }

    //ִ�гɹ� �ͷ�redisCommandִ�к󷵻ص�redisReply��ռ�õ��ڴ�
    freeReplyObject(r);
    printf("Succeed to execute command[%s]\n", command1);

    const char* command2 = "strlen stest1";
    r = (redisReply*)redisCommand(c, command2);

    //����������Ͳ������� ���ͷ�����
    if (r->type != REDIS_REPLY_INTEGER)
    {
        printf("Failed to execute command[%s]\n", command2);
        freeReplyObject(r);
        redisFree(c);        return;
    }

    //��ȡ�ַ�������
    int length = r->integer;
    freeReplyObject(r);
    printf("The length of 'stest1' is %d.\n", length);
    printf("Succeed to execute command[%s]\n", command2);

    //��ȡredis��ֵ����Ϣ
    const char* command3 = "get stest1";
    r = (redisReply*)redisCommand(c, command3);
    if (r->type != REDIS_REPLY_STRING)
    {
        printf("Failed to execute command[%s]\n", command3);
        freeReplyObject(r);
        redisFree(c);        return;
    }
    printf("The value of 'stest1' is %s\n", r->str);
    freeReplyObject(r);
    printf("Succeed to execute command[%s]\n", command3);

    const char* command4 = "get stest2";
    r = (redisReply*)redisCommand(c, command4);
    if (r->type != REDIS_REPLY_NIL)
    {
        printf("Failed to execute command[%s]\n", command4);
        freeReplyObject(r);
        redisFree(c);        return;
    }
    freeReplyObject(r);
    printf("Succeed to execute command[%s]\n", command4);

    //�ͷ�������Դ
    redisFree(c);

}

void TestRedisMgr() {

    assert(RedisMgr::GetInstance()->Set("blogwebsite", "llfc.club"));
    std::string value = "";
    assert(RedisMgr::GetInstance()->Get("blogwebsite", value));
    assert(RedisMgr::GetInstance()->Get("nonekey", value) == false);
    assert(RedisMgr::GetInstance()->HSet("bloginfo", "blogwebsite", "llfc.club"));
    assert(RedisMgr::GetInstance()->HGet("bloginfo", "blogwebsite") != "");
    assert(RedisMgr::GetInstance()->ExistsKey("bloginfo"));
    assert(RedisMgr::GetInstance()->Del("bloginfo"));
    assert(RedisMgr::GetInstance()->Del("bloginfo"));
    assert(RedisMgr::GetInstance()->ExistsKey("bloginfo") == false);
    assert(RedisMgr::GetInstance()->LPush("lpushkey1", "lpushvalue1"));
    assert(RedisMgr::GetInstance()->LPush("lpushkey1", "lpushvalue2"));
    assert(RedisMgr::GetInstance()->LPush("lpushkey1", "lpushvalue3"));
    assert(RedisMgr::GetInstance()->RPop("lpushkey1", value));
    assert(RedisMgr::GetInstance()->RPop("lpushkey1", value));
    assert(RedisMgr::GetInstance()->LPop("lpushkey1", value));
    assert(RedisMgr::GetInstance()->LPop("lpushkey2", value) == false);
    //RedisMgr::GetInstance()->Close();
}

int main()
{
    //TestRedis();
    //ConfigMgr gCfgMgr;
    //TestRedisMgr();
    auto& gCfgMgr = ConfigMgr::Inst();
    std::string gate_port_str = gCfgMgr["GateServer"]["Port"];
    unsigned short gate_port = atoi(gate_port_str.c_str());

    try {
        unsigned short port = static_cast<unsigned short>(8080);
        net::io_context ioc{ 1 };
        boost::asio::signal_set signals(ioc, SIGINT, SIGTERM);
        signals.async_wait([&ioc](const boost::system::error_code& error, int signal_number) {
            if (error) {
                return;
            }
            ioc.stop(); //��ΪҪ�ı�ioc��״̬, ����Ҫ���ò����ֹͣ��ioc, ������ע����߼�
        });

        std::make_shared<CServer>(ioc, port)->Start();
        std::cout << "GateServer listen on port: " << port << std::endl;
        ioc.run();  //������ѯ
    }
    catch (std::exception const& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}









// boost�������Ƿ�ɹ��Ĳ����ļ�
//#include <iostream>
//#include <string>
//#include "boost/lexical_cast.hpp"
//int main()
//{
//    using namespace std;
//    cout << "Enter your weight: ";
//    float weight;
//    cin >> weight;
//    string gain = "A 10% increase raises ";
//    string wt = boost::lexical_cast<string> (weight);
//    gain = gain + wt + " to ";      // string operator()
//    weight = 1.1 * weight;
//    gain = gain + boost::lexical_cast<string>(weight) + ".";
//    cout << gain << endl;
//    system("pause");
//    return 0;
//}

/***********jsoncpp�����Ƿ�ɹ��Ĳ����ļ�****************/