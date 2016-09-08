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
		//接收数据
		if((bytes=recv((SOCKET)lpParameter,buf,sizeof(buf),0))==SOCKET_ERROR){
			TRACE("接收数据失败!\n");
			//_endthread();//终止当前线程
		}
		buf[bytes]='\0';
		//cout<<"客户端说："<<buf<<endl;
	TRACE("recv:%s\n",buf);
	}
}
int CClientNet::Connect( int port,const char* address )
{

	//// 初始化
	//AfxSocketInit();

	////创建 CSocket 对象
	//CSocket aSocket;

	//CString strIP;
	//CString strPort;
	//CString strText;


	////初始化 CSocket 对象, 因为客户端不需要绑定任何端口和地址, 所以用默认参数即可
	//if(!aSocket.Create())
	//{
	//	char szMsg[1024] = {0};

	//	sprintf(szMsg, "create faild: %d", aSocket.GetLastError());

	//	AfxMessageBox(szMsg);
	//	return 0;
	//}

	////转换需要连接的端口内容类型
	////int nPort = atoi(strPort);

	////连接指定的地址和端口
	//if(aSocket.Connect(address, port))
	//{
	//	char szRecValue[1024] = {0};
	//	char send_str[2048] = {0};
	//	char api[]="http://221.12.156.123:16915/issue_tradeweb/httpXmlServlet";
	//	char hostname[]="221.12.156.123:16915";
	//	//头信息
	//	strcat(send_str, "POST ");
	//	strcat(send_str, api);
	//	strcat(send_str, " HTTP/1.1\r\n");
	//	strcat(send_str, "Host: ");
	//	strcat(send_str, hostname);
	//	strcat(send_str, "\r\n");
	//	strcat(send_str, "Accept-Encoding:identity\r\n");
	//	strcat(send_str, "Content-Length:218\r\n\r\n");
	//	strcat(send_str, "<?xml version=\"1.0\" encoding=\"gb2312\"?><GNNT><REQ name=\"logon\"><USER_ID>803300000010212</USER_ID><PASSWORD>418331101</PASSWORD><REGISTER_WORD></REGISTER_WORD><VERSIONINFO></VERSIONINFO><LOGONTYPE>pc</LOGONTYPE></REQ></GNNT>");

	//	//发送内容给服务器
	//	aSocket.Send(send_str, strlen(send_str));

	//	//接收服务器发送回来的内容(该方法会阻塞, 在此等待有内容接收到才继续向下执行)
	//	aSocket.Receive((void *)szRecValue, 1024);

	//	AfxMessageBox(szRecValue);
	//}
	//else
	//{
	//	char szMsg[1024] = {0};

	//	sprintf(szMsg, "create faild: %d", aSocket.GetLastError());
	//	AfxMessageBox(_T("Are you sure?"));
	//}

	////关闭
	////aSocket.Close();
	int err;
	WORD versionRequired;
	WSADATA wsaData;
	versionRequired=MAKEWORD(1,1);
	err=WSAStartup(versionRequired,&wsaData);//协议库的版本信息
	if (!err)
	{
		TRACE("客户端嵌套字已经打开!\n");
	}
	else
	{
		TRACE("客户端的嵌套字打开失败!\n");
		return 0;//结束
	}
	 m_sock=socket(AF_INET,SOCK_STREAM,0);
	SOCKADDR_IN clientsock_in;
	clientsock_in.sin_addr.S_un.S_addr=inet_addr(address);
	clientsock_in.sin_family=AF_INET;
	clientsock_in.sin_port=htons(port);
	 err=connect(m_sock,(SOCKADDR*)&clientsock_in,sizeof(SOCKADDR));//开始连接
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

	//发送消息，指定sock发送消息
	iErrMsg = send(m_sock,msg,len,0);
	if (iErrMsg < 0)
		//发送失败
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
	recv(m_sock, msg, len,0); ///接收
	//AfxMessageBox(recvbuf);
	return 0;
}
void CClientNet::Close()
{
	closesocket(m_sock);
}
int CClientNet::HttpLogin(int accout,int passwds)
{
	 //初始化发送信息
    char send_str[2048] = {0};
	char api[]="http://123.59.182.105:16915/issue_tradeweb/httpXmlServlet";
	char hostname[]="123.59.182.105:16915";

    //头信息
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