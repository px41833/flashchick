// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <WINSOCK2.H>
#include <STDIO.H>
#pragma  comment(lib,"ws2_32.lib")
#define MYPORT  16915
#define IPADDR  "123.59.182.105"
#define BUFFER_SIZE 1024
time_t GetCurrMSForMe() //获取1970年到现在的毫秒数
{
	SYSTEMTIME tTime = {0};
	GetSystemTime(&tTime);

	FILETIME fTime = {0};
	SystemTimeToFileTime(&tTime, &fTime);

	ULARGE_INTEGER ui;
	ui.LowPart = fTime.dwLowDateTime;
	ui.HighPart = fTime.dwHighDateTime;

	return ((LONGLONG)(ui.QuadPart-116444736000000000)/10000);
}
int _tmain(int argc, _TCHAR* argv[])
{
		WORD sockVersion = MAKEWORD(2,2);
		WSADATA data; 
		if(WSAStartup(sockVersion, &data) != 0)
		{
			return 0;
		}

		SOCKET sclient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if(sclient == INVALID_SOCKET)
		{
			printf("invalid socket !");
			return 0;
		}

		sockaddr_in serAddr;
		serAddr.sin_family = AF_INET;
		serAddr.sin_port = htons(16915);
		serAddr.sin_addr.S_un.S_addr = inet_addr("123.59.182.105"); 
		if (connect(sclient, (sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
		{
			printf("connect error !");
			closesocket(sclient);
			return 0;
		}
		char * sendData = "你好，TCP服务端，我是客户端!\n";
		char sendbuf[BUFFER_SIZE];
		char recvbuf[BUFFER_SIZE];
		char retcode[128];

			//初始化发送信息
			char send_str[2048] = {0};
			char api[]="http://123.59.182.105:16915/issue_tradeweb/httpXmlServlet";
			char hostname[]="123.59.182.105:16915";
			// char api[]="http://221.12.156.123:16915/issue_tradeweb/httpXmlServlet";
			//char hostname[]="221.12.156.123:16915";

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
			
			strcat(send_str, "<?xml version=\"1.0\" encoding=\"gb2312\"?><GNNT><REQ name=\"logon\"><USER_ID>1299906727</USER_ID><PASSWORD>418331101</PASSWORD><REGISTER_WORD></REGISTER_WORD><VERSIONINFO></VERSIONINFO><LOGONTYPE>pc</LOGONTYPE></REQ></GNNT>");

			printf("!!!!!!!!!!11step!!!!!!!!!!!!!send_str:%s\n",send_str);
		send(sclient, send_str, strlen(send_str), 0);

		 recv(sclient, recvbuf, sizeof(recvbuf),0); ///接收

		 char *start=strstr(recvbuf,"<RETCODE>");
		 char *end=strstr(recvbuf,"</RETCODE>");
		 memset(retcode, 0, sizeof(retcode));
		 memcpy(retcode,start+sizeof("<RETCODE>")-1,end-start-sizeof("<RETCODE>")+1);
		 printf("!!!!!!!!555!!!!!!!!!!!!!!!RETCODE:%s\n",retcode);
		 memset(sendbuf, 0, sizeof(sendbuf));
		 memset(recvbuf, 0, sizeof(recvbuf));
		 memset(send_str, 0, sizeof(send_str));

		 //头信息
		 strcat(send_str, "POST ");
		 strcat(send_str, api);
		 strcat(send_str, " HTTP/1.1\r\n");
		 strcat(send_str, "Host: ");
		 strcat(send_str, hostname);
		 strcat(send_str, "\r\n");
		 strcat(send_str, "Accept-Encoding:identity\r\n");
		 strcat(send_str, "Content-Length:153\r\n\r\n");

		 strcat(send_str, "<?xml version=\"1.0\" encoding=\"gb2312\"?><GNNT><REQ name=\"firm_info\"><USER_ID>1299906727</USER_ID><SESSION_ID>");
		 strcat(send_str,retcode);
		 strcat(send_str,"</SESSION_ID></REQ></GNNT>");

		 printf("!!!!!!!!!!22step!!!!!!!!!!!!!send_str:%s\n",send_str);

		 send(sclient, send_str, strlen(send_str),0); ///发送
		 //if(strcmp(sendbuf,"exit\n")==0)
		 //  break;
		 recv(sclient, recvbuf, sizeof(recvbuf),0); ///接收
		 fputs(recvbuf, stdout);

		 memset(sendbuf, 0, sizeof(sendbuf));
		 memset(recvbuf, 0, sizeof(recvbuf));
		 memset(send_str, 0, sizeof(send_str));

		 //头信息
		 strcat(send_str, "POST ");
		 strcat(send_str, api);
		 strcat(send_str, " HTTP/1.1\r\n");
		 strcat(send_str, "Host: ");
		 strcat(send_str, hostname);
		 strcat(send_str, "\r\n");
		 strcat(send_str, "Accept-Encoding:identity\r\n");
		 strcat(send_str, "Content-Length:188\r\n\r\n");

		 strcat(send_str, "<?xml version=\"1.0\" encoding=\"gb2312\"?><GNNT><REQ name=\"commodity_query\"><USER_ID>1299906727</USER_ID><COMMODITY_ID></COMMODITY_ID><SESSION_ID>");
		 strcat(send_str,retcode);
		 strcat(send_str,"</SESSION_ID></REQ></GNNT>");

		 printf("!!!!!!!!!!3 step!!!!!!!!!!!!!send_str:%s\n",send_str);

		 /*send(sclient, send_str, strlen(send_str),0); ///发送

		 while(0!=recv(sclient, recvbuf, sizeof(recvbuf),0)) ///接收
		 fputs(recvbuf, stdout);*/

		 memset(sendbuf, 0, sizeof(sendbuf));
		 memset(recvbuf, 0, sizeof(recvbuf));
		 memset(send_str, 0, sizeof(send_str));

		 //头信息
		 strcat(send_str, "POST ");
		 strcat(send_str, api);
		 strcat(send_str, " HTTP/1.1\r\n");
		 strcat(send_str, "Host: ");
		 strcat(send_str, hostname);
		 strcat(send_str, "\r\n");
		 strcat(send_str, "Accept-Encoding:identity\r\n");
		 strcat(send_str, "Content-Length:212\r\n\r\n");

		 strcat(send_str, "<?xml version=\"1.0\" encoding=\"gb2312\"?><GNNT><REQ name=\"sys_time_query\"><USER_ID>1299906727</USER_ID><LAST_ID>0</LAST_ID><TD_CNT>0</TD_CNT><SESSION_ID>");
		 strcat(send_str,retcode);
		 strcat(send_str,"</SESSION_ID><CU_LG>1</CU_LG></REQ></GNNT>");

		 printf("!!!!!!!!!!4 step syn time!!!!!!!!!!!!!send_str:%s\n",send_str);
		 SYSTEMTIME st;

		// CString strDate,strTime;
		 for (int i=0;i<4;i++)
		 {
	
		 send(sclient, send_str, strlen(send_str),0); ///发送

		 recv(sclient, recvbuf, sizeof(recvbuf),0); ///接收
		 fputs(recvbuf, stdout);
		 

		 printf("\r\ntime:%lld\n",GetCurrMSForMe());
		 //GetLocalTime(&st);
		 //printf("\r\ni:%d,time:%u:%u:%u:%d:%d:%d:%d\n",i,st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond,st.wMilliseconds);
		// strDate.Format("M----",st.wYear,st.wMonth,st.wDay);

		 //strTime.Format("-:-:-",st.wHour,st.wMinute,st.wSecond) ;

		 //AfxMessageBox(strDate);

		 //AfxMessageBox(strTime);
		 }
		closesocket(sclient);
		WSACleanup();

	return 0;
}

