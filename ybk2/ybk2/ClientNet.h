#pragma once

/*
ClinetNet.h��
���ڱ�ʾһ���ͻ����������
*/

#include <stdio.h>
#include<windows.h>

#pragma comment(lib, "Ws2_32.lib")

class CClientNet
{
public:
	//������ָ��������
	int Connect(int port,const char* address);
	//������Ϣ
	int SendMsg(const char* msg,int len);

	int RecvMsg(char *msg,int len);
	void ProcXmlDate(char *s);
    int RepCode;  //��¼�ɹ��󷵻ص������
	int RetYuE;//���˺��ڵ����

	//�ر�
	void Close();
public:
	int HttpLogin(int accout,int passwds);


private:
	SOCKET m_sock;
};