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
	void BuildXmlData_Logon(char *s,int accout,int passwd);
	void BuildXmlData_ReqFirmInfo(char *s);
    

	//�ر�
	void Close();
public:
	char hostname[32];
	char UserId[32];
	char Passwd[32];
	char RepCode[64];  //��¼�ɹ��󷵻ص������
	int RetYuE;//���˺��ڵ����
	//int HttpLogin(int accout,int passwds);


private:
	SOCKET m_sock;
};