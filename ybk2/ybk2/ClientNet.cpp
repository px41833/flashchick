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
	versionRequired=MAKEWORD(2,2);
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
	 sprintf_s(hostname,"%s:%d",address,port);
	 TRACE("host:%s\n",hostname);
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
void CClientNet::BuildXmlData_Logon(char *s,int accout,int passwd)
{
	 //��ʼ��������Ϣ
    char send_str[2048] = {0};
	char api[]="http://123.59.182.105:16915/issue_tradeweb/httpXmlServlet";
	//char hostname[]="123.59.182.105:16915";
	
	_itoa_s(accout,UserId,10);
	_itoa_s(passwd,Passwd,10);

    //ͷ��Ϣ
    strcat_s(send_str, "POST ");
    strcat_s(send_str, api);
    strcat_s(send_str, " HTTP/1.1\r\n");
    strcat_s(send_str, "Host: ");
    strcat_s(send_str, hostname);
    strcat_s(send_str, "\r\n");
	strcat_s(send_str, "Accept-Encoding:identity\r\n");
	
	
	strcat_s(send_str, "Content-Length:252\r\n\r\n");
	sprintf(s,"%s<?xml version=\"1.0\" encoding=\"gb2312\"?><GNNT><REQ name=\"logon\"><USER_ID>%d</USER_ID><PASSWORD>%d</PASSWORD><REGISTER_WORD></REGISTER_WORD><VERSIONINFO></VERSIONINFO><LOGONTYPE>pc</LOGONTYPE></REQ></GNNT>",send_str,accout,passwd);
	return ;
	//�����������У�����
	//TiXmlDocument doc;
	//TiXmlElement* root = new TiXmlElement("GNNT");
	//doc.LinkEndChild(root);
	//TiXmlElement* element1 = new TiXmlElement("REQ");
	//root->LinkEndChild(element1);
	//element1->SetAttribute("name", "logon");
	//TiXmlElement* userid = new TiXmlElement("USER_ID");  ///Ԫ��
	//element1->LinkEndChild(userid);

	//TiXmlText* useridtext = new TiXmlText(UserId);  ///�ı�
	//userid->LinkEndChild(useridtext);

	//TiXmlElement* userpwd = new TiXmlElement("PASSWORD");  ///Ԫ��
	//element1->LinkEndChild(userpwd);
	//TiXmlText* passwdtext = new TiXmlText(Passwd);  ///�ı�
	//userpwd->LinkEndChild(passwdtext);

	//TiXmlElement* logontype = new TiXmlElement("LOGONTYPE");  ///Ԫ��
	//element1->LinkEndChild(logontype);
	//TiXmlText* logontypetext = new TiXmlText("pc");  ///�ı�
	//logontype->LinkEndChild(logontypetext);
	//
	//TiXmlPrinter printer;
	//printer.SetIndent( 0 ); // ���������ַ�����Ϊ 0 ��ʾ��ʹ��������Ĭ��Ϊ 4���ո�Ҳ����Ϊ'\t'  
	//doc.Accept( &printer ); 

	//char content[256] = {0};  
	//int size = printer.Size(); 
	//assert( size < sizeof(content) );  
	//strcpy_s( content, sizeof(content), printer.CStr() );  
    
	//		strcat(send_str, "<?xml version=\"1.0\" encoding=\"gb2312\"?><GNNT><REQ name=\"logon\"><USER_ID>1299906727</USER_ID><PASSWORD>418331101</PASSWORD><REGISTER_WORD></REGISTER_WORD><VERSIONINFO></VERSIONINFO><LOGONTYPE>pc</LOGONTYPE></REQ></GNNT>");

	//		SendMsg(send_str,strlen(send_str));
	//		DWORD ThreadID;
	//		//CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThreadFuncRecive,(LPVOID)m_sock,0,&ThreadID);

	  /*     char xmllen[64];
		   sprintf_s(xmllen,"Content-Length:%d\r\n",218);
		   
		   strcat(s, xmllen);
		    strcat(s, "<?xml version=\"1.0\" encoding=\"gb2312\"?>");
		    strcat(s, "\r\n");
		   */
		   
			//strcat_s()
			//strcat(s, content);
		  // sprintf(s,"%s%s%s",send_str,content,"/GNNT");
			//doc.Clear();
			
}
void CClientNet::BuildXmlData_ReqFirmInfo(char *s)
{


	char send_str[2048] = {0};
	char api[]="http://123.59.182.105:16915/issue_tradeweb/httpXmlServlet";
	//char hostname[]="123.59.182.105:16915";


	//ͷ��Ϣ
	strcat_s(send_str, "POST ");
	strcat_s(send_str, api);
	strcat_s(send_str, " HTTP/1.1\r\n");
	strcat_s(send_str, "Host: ");
	strcat_s(send_str, hostname);
	strcat_s(send_str, "\r\n");
	strcat_s(send_str, "Accept-Encoding:identity\r\n");


	strcat_s(send_str, "Content-Length:153\r\n\r\n");
	strcat_s(send_str, "<?xml version=\"1.0\" encoding=\"gb2312\"?><GNNT><REQ name=\"firm_info\"><USER_ID>1299906727</USER_ID><SESSION_ID>");
	strcat_s(send_str,RepCode);
	strcat_s(send_str,"</SESSION_ID></REQ></GNNT>");
	memcpy(s,send_str,sizeof(send_str));
	//sprintf(s,"%s<?xml version=\"1.0\" encoding=\"gb2312\"?><GNNT><REQ name=\"firm_info\"><USER_ID>1299906727</USER_ID><SESSION_ID>%s</SESSION_ID></REQ></GNNT>",send_str,RepCode);
	return ;
}
void CClientNet::ProcXmlDate(char *s)
{
	//�˴����յ�������HTTPͷ��Ҫȥ��
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
	const char *retcode=databaseElement->GetText();
	strcpy_s(RepCode,retcode);
	//AfxMessageBox(RepCode);
}
void CClientNet::ProcXmlDateEx(char *s)
{
	//�˴����յ�������HTTPͷ��Ҫȥ��
	//char *xmldata=strchr(s,'<');
	char *start=strstr(s,"<RETCODE>");
	char *end=strstr(s,"</RETCODE>");
	memset(RepCode, 0, sizeof(RepCode));
	memcpy(RepCode,start+sizeof("<RETCODE>")-1,end-start-sizeof("<RETCODE>")+1);
	//strcpy_s(RepCode,retcode);
	//AfxMessageBox(RepCode);

		///////////////////////////////////////////////////////////////////
	//ͷ��Ϣ
	char send_str[2048] = {0};
	memset(send_str, 0, sizeof(send_str));


	char appi[]="http://123.59.182.105:16915/issue_tradeweb/httpXmlServlet";
	strcat(send_str, "POST ");
	strcat(send_str, appi);
	strcat(send_str, " HTTP/1.1\r\n");
	strcat(send_str, "Host: ");
	strcat(send_str, hostname);
	strcat(send_str, "\r\n");
	strcat(send_str, "Accept-Encoding:identity\r\n");
	strcat(send_str, "Content-Length:153\r\n\r\n");

	strcat(send_str, "<?xml version=\"1.0\" encoding=\"gb2312\"?><GNNT><REQ name=\"firm_info\"><USER_ID>1299906727</USER_ID><SESSION_ID>");
	strcat(send_str,RepCode);
	strcat(send_str,"</SESSION_ID></REQ></GNNT>");
	//SendMsg(send_str,strlen(send_str));
	send(m_sock, send_str, strlen(send_str),0); ///����
	    char recvbuf[1024];
		memset(recvbuf, 0, sizeof(recvbuf));
	recv(m_sock, recvbuf, sizeof(recvbuf),0); ///����

	TRACE("recv:%s\n",recvbuf);
}