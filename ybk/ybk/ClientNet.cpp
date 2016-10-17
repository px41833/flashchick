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
	time_localcurrent=0;
	settimediff=0;
	cm_check=FALSE;
	m_WatchDog=0;
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
void ClientNet::DisConnect()
{
	closesocket(m_sock);
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
	FD_ZERO(&fds); //每次循环都要清空集合，否则不能检测描述符变化 
	u_long ul=1;
	ioctlsocket(client->m_sock,FIONBIO,&ul);    //用非阻塞的连接
	FD_SET(client->m_sock,&fds); //添加描述符 
	int s_tick_stats=0;
    while(client->RunFlag) 
   { 
        
        //maxfdp=sock>fp?sock+1:fp+1;    //描述符最大值加1 
        ret=(select(NULL,&fds,NULL,NULL,&timeout));   //select使用 
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
							client->m_WatchDog=0;//喂狗
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
							client->m_WatchDog=0;//喂狗
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
							client->m_WatchDog=0;//喂狗
							int ret=PostThreadMessage(client->MainWinThreadID,MESSAGE_FIRM_INFO,(WPARAM)revstart,0);
							//TRACE(" sync time ret:%d\n",ret);
						}
					}
					else
					{
						TRACE("~~error code0x%x\n",WSAGetLastError());
						s_tick_stats++;
						if (s_tick_stats>10)
						{
							client->m_WatchDog=0;//喂狗
							s_tick_stats=0;
							//发送消息进行重新连接
							int ret=PostThreadMessage(client->MainWinThreadID,MESSAGE_RE_LOGON,NULL,0);
							break;
						}
					}
					
				}
			}

        }
	
     }//end while 
	TRACE("client exit\n");
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
	RunFlag=TRUE;
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
	logonstr +="</PASSWORD><REGISTER_WORD>20161017145113403129990672748216.16447345315</REGISTER_WORD><VERSIONINFO></VERSIONINFO><LOGONTYPE>pc</LOGONTYPE></REQ></GNNT>";
	lenthstr.Format(_T("Content-Length:%d\r\n"),logonstr.GetLength());
	lenthstr +="Connection: Keep-Alive\r\n\r\n";
	s=XmlHead+lenthstr+logonstr;
	USES_CONVERSION;
	SendMsg((const char*)T2A(s),s.GetLength());
	
	return s;
}
CString ClientNet::BuildXmlData_CheckUser(CString s,int len)
{
	CString logonstr,lenthstr;
	logonstr.Empty();
	logonstr ="<?xml version=\"1.0\" encoding=\"gb2312\"?><GNNT><REQ name=\"check_user\"><USER_ID>";
	logonstr +=LoginAccout;
	logonstr +="</USER_ID><SESSION_ID>";
	logonstr +=this->RetRandCode;
	logonstr +="</SESSION_ID><MODULE_ID>18</MODULE_ID><F_LOGONTYPE></F_LOGONTYPE><LOGONTYPE>pc</LOGONTYPE></REQ></GNNT>";
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
CString ClientNet::BuildXmlData_Holding(CString s,int len)
{
	CString logonstr,lenthstr;
	logonstr.Empty();
	logonstr ="<?xml version=\"1.0\" encoding=\"gb2312\"?><GNNT><REQ name=\"holding_query\"><USER_ID>";
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
CString ClientNet::BuildXmlData_DataQuery(CString s,int len,CString querycode)
{
	CString logonstr,lenthstr;
	logonstr.Empty();
	logonstr ="<?xml version=\"1.0\" encoding=\"gb2312\"?><GNNT><REQ name=\"commodity_data_query\"><USER_ID>";
	logonstr +=LoginAccout;
	logonstr +="</USER_ID><COMMODITY_ID>";
	logonstr +=querycode;
	logonstr +="</COMMODITY_ID><SESSION_ID>";
	logonstr +=this->RetRandCode;
	logonstr +="</SESSION_ID></REQ></GNNT>";
	lenthstr.Format(_T("Content-Length:%d\r\n"),logonstr.GetLength());
	lenthstr +="Connection: Keep-Alive\r\n\r\n";
	s=XmlHead+lenthstr+logonstr;
	USES_CONVERSION;
	SendMsg((const char*)T2A(s),s.GetLength());

	return s;
}
CString ClientNet::BuildXmlData_Commit(CString s,CString querycode,CString direct,CString price,CString num)
{
	CString logonstr,lenthstr;
	logonstr.Empty();
	logonstr ="<?xml version=\"1.0\" encoding=\"gb2312\"?><GNNT><REQ name=\"order\"><USER_ID>";
	logonstr +=LoginAccout;
	logonstr +="</USER_ID><CUSTOMER_ID>";
	logonstr +=LoginAccout;
	logonstr +="00";
	logonstr +="</CUSTOMER_ID><BUY_SELL>";
	logonstr +=direct;
	logonstr +="</BUY_SELL><COMMODITY_ID>";
	logonstr +="99";
	logonstr +=querycode;
	logonstr +="</COMMODITY_ID><PRICE>";
	logonstr +=price;
	logonstr +="</PRICE><QTY>";
	logonstr +=num;
	logonstr +="</QTY><SETTLE_BASIS>1</SETTLE_BASIS><CLOSEMODE>0</CLOSEMODE><TIMEFLAG>0</TIMEFLAG><L_PRICE>0</L_PRICE><SESSION_ID>";

	logonstr +=this->RetRandCode;
	logonstr +="</SESSION_ID><BILLTYPE>0</BILLTYPE></REQ></GNNT>";
	lenthstr.Format(_T("Content-Length:%d\r\n"),logonstr.GetLength());
	lenthstr +="Connection: Keep-Alive\r\n\r\n";
	s=XmlHead+lenthstr+logonstr;
	USES_CONVERSION;
	SendMsg((const char*)T2A(s),s.GetLength());
	TRACE("commit:%s\n",( char*)T2A(s));
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
int ClientNet::SetCommitTime(COM_TIME cmtime)
{
	cm_check=cmtime.com_check;
	settimediff=cmtime.settimediff;
	time_localcurrent=cmtime.time_localcurrent;
    ListCommit=TRUE;
	return TRUE;
}
void ClientNet::StartCommitList(void)
{
	CString commit;
	vector<YB_PARAM>::iterator it;
	for(it=yb_vec.begin();it!=yb_vec.end();it++)
	{
		BuildXmlData_Commit(commit,it->yb_code,it->yb_sale,it->yb_price,it->yb_number);
		TRACE("%s,%s,%s,%s,%d\n",it->yb_code,it->yb_price,it->yb_sale,it->yb_number,yb_vec.size());
	}
	ListCommit=FALSE;
}
int ClientNet::GetCommitListLen(void)
{
	return yb_vec.empty();
}