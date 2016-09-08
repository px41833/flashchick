#pragma once

/*
ClinetNet.h：
用于表示一个客户的网络操作
*/

#include <stdio.h>
#include<windows.h>

#pragma comment(lib, "Ws2_32.lib")

class CClientNet
{
public:
	//连接上指定服务器
	int Connect(int port,const char* address);
	//发送信息
	int SendMsg(const char* msg,int len);

	int RecvMsg(char *msg,int len);
	void ProcXmlDate(char *s);
    int RepCode;  //登录成功后返回的随机码
	int RetYuE;//此账号内的余额

	//关闭
	void Close();
public:
	int HttpLogin(int accout,int passwds);


private:
	SOCKET m_sock;
};