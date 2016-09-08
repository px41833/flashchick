/*
CLientNet.cpp
*/
#include "stdafx.h"
#include "ClientNet.h"
#include "tinyxml.h"
#include  <afxsock.h>
#include <WINSOCK2.H>

void ThreadFuncRecive(LPVOID lpParameter)
{
	char buf[2048];
	int bytes;
	while(1)
	{
		//��������
		if((bytes=recv((SOCKET)lpParameter,buf,sizeof(buf),0))==SOCKET_ERROR){
			TRACE("��������ʧ��!\n");
			//_endthread();//��ֹ��ǰ�߳�
		}
		buf[bytes]='\0';
		//cout<<"�ͻ���˵��"<<buf<<endl;
	TRACE("recv:%s\n",buf);
	}
}
int CClientNet::Connect( int port,const char* address )
{

	//// ��ʼ��
	//AfxSocketInit();

	////���� CSocket ����
	//CSocket aSocket;

	//CString strIP;
	//CString strPort;
	//CString strText;


	////��ʼ�� CSocket ����, ��Ϊ�ͻ��˲���Ҫ���κζ˿ں͵�ַ, ������Ĭ�ϲ�������
	//if(!aSocket.Create())
	//{
	//	char szMsg[1024] = {0};

	//	sprintf(szMsg, "create faild: %d", aSocket.GetLastError());

	//	AfxMessageBox(szMsg);
	//	return 0;
	//}

	////ת����Ҫ���ӵĶ˿���������
	////int nPort = atoi(strPort);

	////����ָ���ĵ�ַ�Ͷ˿�
	//if(aSocket.Connect(address, port))
	//{
	//	char szRecValue[1024] = {0};
	//	char send_str[2048] = {0};
	//	char api[]="http://221.12.156.123:16915/issue_tradeweb/httpXmlServlet";
	//	char hostname[]="221.12.156.123:16915";
	//	//ͷ��Ϣ
	//	strcat(send_str, "POST ");
	//	strcat(send_str, api);
	//	strcat(send_str, " HTTP/1.1\r\n");
	//	strcat(send_str, "Host: ");
	//	strcat(send_str, hostname);
	//	strcat(send_str, "\r\n");
	//	strcat(send_str, "Accept-Encoding:identity\r\n");
	//	strcat(send_str, "Content-Length:218\r\n\r\n");
	//	strcat(send_str, "<?xml version=\"1.0\" encoding=\"gb2312\"?><GNNT><REQ name=\"logon\"><USER_ID>803300000010212</USER_ID><PASSWORD>418331101</PASSWORD><REGISTER_WORD></REGISTER_WORD><VERSIONINFO></VERSIONINFO><LOGONTYPE>pc</LOGONTYPE></REQ></GNNT>");

	//	//�������ݸ�������
	//	aSocket.Send(send_str, strlen(send_str));

	//	//���շ��������ͻ���������(�÷���������, �ڴ˵ȴ������ݽ��յ��ż�������ִ��)
	//	aSocket.Receive((void *)szRecValue, 1024);

	//	AfxMessageBox(szRecValue);
	//}
	//else
	//{
	//	char szMsg[1024] = {0};

	//	sprintf(szMsg, "create faild: %d", aSocket.GetLastError());
	//	AfxMessageBox(_T("Are you sure?"));
	//}

	////�ر�
	////aSocket.Close();
	int err;
	WORD versionRequired;
	WSADATA wsaData;
	versionRequired=MAKEWORD(1,1);
	err=WSAStartup(versionRequired,&wsaData);//Э���İ汾��Ϣ
	if (!err)
	{
		TRACE("�ͻ���Ƕ�����Ѿ���!\n");
	}
	else
	{
		TRACE("�ͻ��˵�Ƕ���ִ�ʧ��!\n");
		return 0;//����
	}
	 m_sock=socket(AF_INET,SOCK_STREAM,0);
	SOCKADDR_IN clientsock_in;
	clientsock_in.sin_addr.S_un.S_addr=inet_addr(address);
	clientsock_in.sin_family=AF_INET;
	clientsock_in.sin_port=htons(port);
	 err=connect(m_sock,(SOCKADDR*)&clientsock_in,sizeof(SOCKADDR));//��ʼ����
	 if (err < 0)
	 {
		 TRACE("connect failed with error : %d\n",err);

		
	 }
	return 0;
	
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
		TRACE("send msg failed with error : %d\n",iErrMsg);

		rlt = 1;
		return rlt;
	}

	return rlt;
}
int CClientNet::RecvMsg( char* msg,int len)
{
	//char recvbuf[2048]={0};
	recv(m_sock, msg, len,0); ///����
	//AfxMessageBox(recvbuf);
	return 0;
}
void CClientNet::Close()
{
	closesocket(m_sock);
}
int CClientNet::HttpLogin(int accout,int passwds)
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
	strcat(send_str, "Content-Length:218\r\n\r\n");

	char content_header[100];
    //sprintf(content_header,"Content-Length: %d\r\n", strlen(parameters));
	const char* demoStart =
		"<?xml version=\"1.0\"  encoding=\"gb2312\"? >\n"
		"<GNNT>\n"
		"<REQ name=\"logon\">\n"
		"<USER_ID>1299906727</USER_ID>\n"
		"<PASSWORD>418331101</PASSWORD>\n"
		"<REGISTER_WORD />\n"
		"<VERSIONINFO />\n"
		"<LOGONTYPE>pc</LOGONTYPE> \n"
		"</REQ> \n"
		"</GNNT> \n";
	TiXmlDocument doc( "pxtest.xml" );
			doc.Parse( demoStart );
			// strcat(send_str, demoStart);
			strcat(send_str, "<?xml version=\"1.0\" encoding=\"gb2312\"?><GNNT><REQ name=\"logon\"><USER_ID>1299906727</USER_ID><PASSWORD>418331101</PASSWORD><REGISTER_WORD></REGISTER_WORD><VERSIONINFO></VERSIONINFO><LOGONTYPE>pc</LOGONTYPE></REQ></GNNT>");

			SendMsg(send_str,strlen(send_str));
			DWORD ThreadID;
			//CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThreadFuncRecive,(LPVOID)m_sock,0,&ThreadID);
			//Sleep(1000);
			

	return 0;
}
void CClientNet::Close()