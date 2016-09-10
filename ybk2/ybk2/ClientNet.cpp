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
	 sprintf_s(hostname,"%s:%d",address,port);
	 TRACE("host:%s\n",hostname);
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
void CClientNet::BuildXmlData_Logon(char *s,int accout,int passwd)
{
	 //初始化发送信息
    char send_str[2048] = {0};
	char api[]="http://123.59.182.105:16915/issue_tradeweb/httpXmlServlet";
	//char hostname[]="123.59.182.105:16915";
	
	itoa(accout,UserId,10);
	itoa(passwd,Passwd,10);
    //头信息
    strcat(s, "POST ");
    strcat(s, api);
    strcat(s, " HTTP/1.1\r\n");
    strcat(s, "Host: ");
    strcat(s, hostname);
    strcat(s, "\r\n");
	strcat(s, "Accept-Encoding:identity\r\n");
	
	TiXmlDocument doc;
	TiXmlElement root("GNNT");  
	TiXmlElement firstChild("REQ"); 
	firstChild.SetAttribute( "name", "logon" ); 

	TiXmlElement userid("USER_ID");
	//firstChild.InsertEndChild( userid ); 
	TiXmlText usridtext(UserId);
	userid.LinkEndChild(&usridtext);
	firstChild.InsertEndChild( userid );

	TiXmlElement userpwd("PASSWORD");
	TiXmlText passwdtext(Passwd);
	userpwd.LinkEndChild(&passwdtext);
    firstChild.InsertEndChild( userpwd );

	TiXmlElement logontype("LOGONTYPE");
	TiXmlText logontypetext("pc");
	logontype.LinkEndChild(&logontypetext);
	firstChild.InsertEndChild( logontype );

	root.InsertEndChild( firstChild ); 
	doc.InsertEndChild( root );  
	char content_header[100];
	TiXmlPrinter printer;
	printer.SetIndent( 0 ); // 设置缩进字符，设为 0 表示不使用缩进。默认为 4个空格，也可设为'\t'  
	doc.Accept( &printer ); 

	char content[256] = {0};  
	int size = printer.Size(); 
	assert( size < sizeof(content) );  
	strcpy_s( content, sizeof(content), printer.CStr() );  
    //sprintf(content_header,"Content-Length: %d\r\n", strlen(parameters));
	//const char* demoStart =
	//	"<?xml version=\"1.0\"  encoding=\"gb2312\"? >\n"
	//	"<GNNT>\n"
	//	"<REQ name=\"logon\">\n"
	//	"<USER_ID>1299906727</USER_ID>\n"
	//	"<PASSWORD>418331101</PASSWORD>\n"
	//	"<REGISTER_WORD />\n"
	//	"<VERSIONINFO />\n"
	//	"<LOGONTYPE>pc</LOGONTYPE> \n"
	//	"</REQ> \n"
	//	"</GNNT> \n";
	//TiXmlDocument doc( "pxtest.xml" );
	//		doc.Parse( demoStart );
	//		// strcat(send_str, demoStart);
	//		strcat(send_str, "<?xml version=\"1.0\" encoding=\"gb2312\"?><GNNT><REQ name=\"logon\"><USER_ID>1299906727</USER_ID><PASSWORD>418331101</PASSWORD><REGISTER_WORD></REGISTER_WORD><VERSIONINFO></VERSIONINFO><LOGONTYPE>pc</LOGONTYPE></REQ></GNNT>");

	//		SendMsg(send_str,strlen(send_str));
	//		DWORD ThreadID;
	//		//CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThreadFuncRecive,(LPVOID)m_sock,0,&ThreadID);
	//		//Sleep(1000);
//	 sprintf_s()
	       char xmllen[64];
		   sprintf_s(xmllen,"Content-Length:%d",stelen);
			strcat(s, "Content-Length:218\r\n\r\n");
			
}
void CClientNet::ProcXmlDate(char *s)
{
	//此处接收的数据有HTTP头需要去掉
	char *xmldata=strchr(s,'<');
	TiXmlDocument doc;
	doc.Parse(xmldata);
	TiXmlElement* databaseElement = 0;  
	TiXmlElement* listElement = 0;
	TiXmlElement* channelElement = 0;
	TiXmlHandle docHandle(&doc);
	databaseElement = docHandle.FirstChild( "GNNT" ).FirstChild( "REP" ).ToElement();
	assert( databaseElement  ); 
	const char *name = databaseElement->Attribute("name");  
	 TRACE("name:%s",name);

	 TiXmlHandle docHandle1 = docHandle.FirstChild( "GNNT" ).FirstChild( "REP" ).FirstChild("RESULT");
	 databaseElement= docHandle1.FirstChildElement("RETCODE").ToElement();
	//char *port = {0};  
	 RepCode=databaseElement->GetText();
	AfxMessageBox(RepCode);
}