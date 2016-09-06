/*
CLientNet.cpp
*/
#include "stdafx.h"
#include "ClientNet.h"
#include "tinyxml.h"

int CClientNet::Connect( int port,const char* address )
{
	int rlt = 0;

	//���ڼ�¼������Ϣ�����
	int iErrMsg;
	//����WinSock
	WSAData wsaData;
	iErrMsg = WSAStartup(MAKEWORD(1,1),&wsaData);
	if (iErrMsg != NO_ERROR)
		//�д���
	{
		printf("failed with wsaStartup error : %d\n",iErrMsg);

		rlt = 1;
		return rlt;
	}
	TRACE("~~~~~~~~~~~~~\n");
	//����Socket
	m_sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if (m_sock == INVALID_SOCKET)
		//����Socketʧ��
	{
		printf("socket failed with error : %d\n",WSAGetLastError());

		rlt = 2;
		return rlt;
	}

	//Ŀ�����������
	sockaddr_in sockaddrServer;
	sockaddrServer.sin_family = AF_INET;
	sockaddrServer.sin_port = port;
	sockaddrServer.sin_addr.s_addr = inet_addr(address);

	//����,sock��Ŀ�����������
	iErrMsg = connect(m_sock,(sockaddr*)&sockaddrServer,sizeof(sockaddrServer));
	if (iErrMsg < 0)
	{
		printf("connect failed with error : %d\n",iErrMsg);

		rlt = 3;
		return rlt;
	}

	return rlt;
}




int CClientNet::SendMsg(const char* msg,int len)
{
	int rlt = 0;

	int iErrMsg = 0;

	//������Ϣ��ָ��sock������Ϣ
	iErrMsg = send(m_sock,msg,len,0);
	if (iErrMsg < 0)
		//����ʧ��
	{
		printf("send msg failed with error : %d\n",iErrMsg);

		rlt = 1;
		return rlt;
	}

	return rlt;
}

void CClientNet::Close()
{
	closesocket(m_sock);
}
int CClientNet::httplogin(int accout,int passwds)
{
	 //��ʼ��������Ϣ
    char send_str[2048] = {0};
	char api[]="http://123.59.182.105:16915/issue_tradeweb/httpXmlServlet";
	char hostname[]="123.59.182.105:16915";

    //ͷ��Ϣ
    strcat(send_str, "POST ");
    strcat(send_str, api);
    strcat(send_str, " HTTP/1.1\r\n");
    strcat(send_str, "Host: ");
    strcat(send_str, hostname);
    strcat(send_str, "\r\n");
	strcat(send_str, "Accept-Encoding:identity\r\n");

	char content_header[100];
    //sprintf(content_header,"Content-Length: %d\r\n", strlen(parameters));
	const char* demoStart =
		"<?xml version=\"1.0\"  standalone='no' >\n"
		"<!-- Our to do list data -->"
		"<ToDo>\n"
		"<!-- Do I need a secure PDA? -->\n"
		"<Item priority=\"1\" distance='close'> Go to the <bold>Toy store!</bold></Item>"
		"<Item priority=\"2\" distance='none'> Do bills   </Item>"
		"<Item priority=\"2\" distance='far &amp; back'> Look for Evil Dinosaurs! </Item>"
		"</ToDo>";
	TiXmlDocument doc( "pxtest.xml" );
			doc.Parse( demoStart );

	return 0;
}