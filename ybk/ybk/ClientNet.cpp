#include "StdAfx.h"
#include <vector>
using namespace std;
#include "common.h"
#include "ClientNet.h"

ClientNet::ClientNet(void)
{
}
ClientNet::ClientNet(CString accout,CString passwd ,CString ipaddr,CString port,DWORD threadid)
{
	LoginAccout=accout;
	LoginPasswd=passwd;
	ConIpAddr=ipaddr;
	ConPort=port;
	MainWinThreadID=threadid;
	RunFlag=TRUE;
	InitXmlData_Head();
	ListCommit=FALSE;
	yb_vec.clear();
}

ClientNet::~ClientNet(void)
{
	TRACE("ClientNet exit\n");
}
int ClientNet::Connect(int port,const char* address)
{
	m_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(m_sock == INVALID_SOCKET)
	{
		TRACE("invalid socket !");
		return FALSE;
	}
	sockaddr_in serAddr;
	serAddr.sin_family = AF_INET;
	serAddr.sin_port = htons(port);
	serAddr.sin_addr.S_un.S_addr = inet_addr(address); 
	if (connect(m_sock, (sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
	{
		printf("connect error !");
		closesocket(m_sock);
		return FALSE;
	}
	return TRUE;
}
void ThreadFuncRecv(LPVOID lpParameter)
{ 
    //int sock=(int)*((int *)lpParameter); 
	ClientNet *client= (ClientNet *)lpParameter;
    struct fd_set fds; 
    struct timeval timeout={3,0}; //select等待3秒，3秒轮询，要非阻塞就置0 
    char buffer[1024]={0}; //256字节的接收缓冲区 
	int ret,revlen=0; // return val 
	char *revstart;
    while(client->RunFlag) 
   { 
        FD_ZERO(&fds); //每次循环都要清空集合，否则不能检测描述符变化 
        FD_SET(client->m_sock,&fds); //添加描述符 
        //maxfdp=sock>fp?sock+1:fp+1;    //描述符最大值加1 
        ret=(select(NULL,&fds,NULL,NULL,NULL));   //select使用 
        if(ret>0)
        {
			memset(buffer,0,sizeof(buffer));
			for  ( int i=0;i<(int)fds.fd_count;i++) 
			{
				if  ( fds.fd_array[ i]== client->m_sock) 
				{
					revlen=recv(client->m_sock,buffer,1024,0);//接受网络数据 
					TRACE("buffer:%s\n",buffer);
					if(revlen>0)
					{
						if (revstart=strstr(buffer,"<REP name=\"logon\">"))
						{
							char *start=strstr(buffer,"<RETCODE>");
							char *end=strstr(buffer,"</RETCODE>");
							char retcode[64];
							memset(retcode, 0, sizeof(retcode));
							memcpy(retcode,start+sizeof("<RETCODE>")-1,end-start-sizeof("<RETCODE>")+1);
							client->RetRandCode=retcode;
							/*PostThreadMessage(client->MainWinThreadID,MESSAGE_LOGON_SUCCESS,(WPARAM)retcode,0);*/
							PostThreadMessage(client->MainWinThreadID,MESSAGE_LOGON_SUCCESS,(WPARAM)revstart,0);
						}
						else if (revstart=strstr(buffer,"<REP name=\"sys_time_query\">"))
						{
							//TRACE("select sync time\n");
							//recv(client->m_sock,buffer,1024,0);//接受网络数据 
							TRACE(" sync time:%s\n",buffer);
							/*char *start=strstr(buffer,"<TV_U>");
							char *end=strstr(buffer,"</TV_U>");
							char retcode[64];
							memset(retcode, 0, sizeof(retcode));
							memcpy(retcode,start+sizeof("<TV_U>")-1,end-start-sizeof("<TV_U>")+1);
							client->SvnMil=retcode;*/
							int ret=PostThreadMessage(client->MainWinThreadID,MESSAGE_SYNC_TIME,(WPARAM)revstart,0);
							TRACE(" sync time ret:%d\n",ret);
						}
						else if (revstart=strstr(buffer,"<REP name=\"firm_info\">"))
						{
							//TRACE("select sync time\n");
							//recv(client->m_sock,buffer,1024,0);//接受网络数据 
							TRACE(" firm_info:%s\n",buffer);
							/*char *start=strstr(buffer,"<TV_U>");
							char *end=strstr(buffer,"</TV_U>");
							char retcode[64];
							memset(retcode, 0, sizeof(retcode));
							memcpy(retcode,start+sizeof("<TV_U>")-1,end-start-sizeof("<TV_U>")+1);
							client->SvnMil=retcode;*/
							int ret=PostThreadMessage(client->MainWinThreadID,MESSAGE_FIRM_INFO,(WPARAM)revstart,0);
							//TRACE(" sync time ret:%d\n",ret);
						}
					}
					
				}
			}

        }
	
     }//end while 
}//end ThreadFuncRecv 

int ClientNet::Connect(void)
{
	m_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(m_sock == INVALID_SOCKET)
	{
		TRACE("invalid socket !");
		return FALSE;
	}
	sockaddr_in serAddr;
	serAddr.sin_family = AF_INET;
	serAddr.sin_port = htons(_ttoi(ConPort));
	USES_CONVERSION;
	
	serAddr.sin_addr.S_un.S_addr = inet_addr((const char*)T2A(ConIpAddr)); 
	if (connect(m_sock, (sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
	{
		TRACE("connect error !");
		closesocket(m_sock);
		return FALSE;
	}
	//成功后创建接收线程
	
	CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThreadFuncRecv,this,0,&ClientThreadID);
	return TRUE;
}
int ClientNet::SendMsg(const char* msg,int len)
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
int ClientNet::RecvMsg( char* msg,int len)
{
	//char recvbuf[2048]={0};
	recv(m_sock, msg, len,0); ///接收
	//AfxMessageBox(recvbuf);
	return 0;
}
void ClientNet::Close()
{
	closesocket(m_sock);
}
CString ClientNet::InitXmlData_Head()
{
	CString xmlheadstr;
	xmlheadstr.Empty();
	xmlheadstr ="POST ";
	xmlheadstr +="/issue_tradeweb/httpXmlServlet ";
	xmlheadstr +=" HTTP/1.1\r\n";
	xmlheadstr +="Content-Type: application/x-www-form-urlencoded\r\n";
	xmlheadstr +="Host: ";
	xmlheadstr +=ConIpAddr;
	xmlheadstr +=":";
	xmlheadstr +=ConPort;
	xmlheadstr +="\r\n";
	XmlHead=xmlheadstr;
	return xmlheadstr;
}
CString ClientNet::BuildXmlData_Logon(CString s,int len)
{
	CString logonstr,lenthstr;
	logonstr.Empty();
	logonstr ="<?xml version=\"1.0\" encoding=\"gb2312\"?><GNNT><REQ name=\"logon\"><USER_ID>";
	logonstr +=LoginAccout;
	logonstr +="</USER_ID><PASSWORD>";
	logonstr +=LoginPasswd;
	logonstr +="</PASSWORD><REGISTER_WORD>201609252116332730180000000015458007.965574113085</REGISTER_WORD><VERSIONINFO></VERSIONINFO><LOGONTYPE>pc</LOGONTYPE></REQ></GNNT>";
	lenthstr.Format(_T("Content-Length:%d\r\n"),logonstr.GetLength());
	lenthstr +="Connection: Keep-Alive\r\n\r\n";
	s=XmlHead+lenthstr+logonstr;
	USES_CONVERSION;
	SendMsg((const char*)T2A(s),s.GetLength());
	
	return s;
}
CString ClientNet::BuildXmlData_GetSvnTime(CString s,int len)
{
	CString logonstr,lenthstr;
	logonstr.Empty();
	logonstr ="<?xml version=\"1.0\" encoding=\"gb2312\"?><GNNT><REQ name=\"sys_time_query\"><USER_ID>";
	logonstr +=LoginAccout;
	logonstr +="</USER_ID><LAST_ID>0</LAST_ID><TD_CNT>0</TD_CNT><SESSION_ID>";
	logonstr +=this->RetRandCode;
	logonstr +="</SESSION_ID><CU_LG>1</CU_LG></REQ></GNNT>";
	lenthstr.Format(_T("Content-Length:%d\r\n"),logonstr.GetLength());
	lenthstr +="Connection: Keep-Alive\r\n\r\n";
	s=XmlHead+lenthstr+logonstr;
	USES_CONVERSION;
	SendMsg((const char*)T2A(s),s.GetLength());

	return s;
}
CString ClientNet::BuildXmlData_GetFirmInfo(CString s,int len)
{
	CString logonstr,lenthstr;
	logonstr.Empty();
	logonstr ="<?xml version=\"1.0\" encoding=\"gb2312\"?><GNNT><REQ name=\"firm_info\"><USER_ID>";
	logonstr +=LoginAccout;
	logonstr +="</USER_ID><SESSION_ID>";
	logonstr +=this->RetRandCode;
	logonstr +="</SESSION_ID></REQ></GNNT>";
	lenthstr.Format(_T("Content-Length:%d\r\n"),logonstr.GetLength());
	lenthstr +="Connection: Keep-Alive\r\n\r\n";
	s=XmlHead+lenthstr+logonstr;
	USES_CONVERSION;
	SendMsg((const char*)T2A(s),s.GetLength());

	return s;
}
void ClientNet::RunTimeCommit(void)
{
	while(RunFlag)
	{
		TRACE("test run\n");
		Sleep(4*500);
		CString synctime;
		BuildXmlData_GetSvnTime(synctime,0);
	}
}
int ClientNet::AddCommitList(YB_PARAM ybk)
{
	yb_vec.push_back(ybk);
	return TRUE;
}