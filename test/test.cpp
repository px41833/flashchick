// test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <WINSOCK2.H>
#include <STDIO.H>
#include  <iostream>
#pragma  comment(lib,"ws2_32.lib")
#define MYPORT  16915
#define IPADDR  "123.59.182.105"
#define BUFFER_SIZE 1024
SOCKET sclient;
char api[]="http://123.59.182.105:16915/issue_tradeweb/httpXmlServlet";
char hostname[]="123.59.182.105:16915";
char retcode[128];

char sertime[128];
long diff=0;
using namespace std;
time_t GetCurrMSForMe() //��ȡ1970�굽���ڵĺ�����
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

time_t GetSetTimeForMe() //��ȡ1970�굽���ڵĺ�����
{
	SYSTEMTIME tTime = {0};
	GetSystemTime(&tTime);
	tTime.wDay=21;
	tTime.wHour=22;
	tTime.wMinute=18;
	tTime.wSecond=0;
	tTime.wMilliseconds=0;

	FILETIME fTime = {0};
	SystemTimeToFileTime(&tTime, &fTime);

	ULARGE_INTEGER ui;
	ui.LowPart = fTime.dwLowDateTime;
	ui.HighPart = fTime.dwHighDateTime;

	return ((LONGLONG)(ui.QuadPart-116444736000000000)/10000);
}
void ThreadFuncSycTime(LPVOID lpParameter)
{
	char sendbuf[BUFFER_SIZE];
	char recvbuf[BUFFER_SIZE];


	//��ʼ��������Ϣ
	char send_str[2048] = {0};
	memset(sendbuf, 0, sizeof(sendbuf));
	memset(recvbuf, 0, sizeof(recvbuf));
	memset(send_str, 0, sizeof(send_str));

	//ͷ��Ϣ
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
	while(1)
	{

		send(sclient, send_str, strlen(send_str),0); ///����

		recv(sclient, recvbuf, sizeof(recvbuf),0); ///����
		fputs(recvbuf, stdout);

		char *start=strstr(recvbuf,"<TV_U>");
		char *end=strstr(recvbuf,"</TV_U>");
		memset(sertime, 0, sizeof(sertime));
		memcpy(sertime,start+sizeof("<TV_U>")-1,end-start-sizeof("<TV_U>")+1);
		printf("\nsertime:%s\n",sertime);
		LONGLONG timea;
		sscanf(sertime,"%lld",&timea);
		printf("\nsertimelong:%lld\n",timea);
		diff=GetCurrMSForMe()-timea;
		//printf("\r\ntime:%lld\n",GetCurrMSForMe());
		printf("\r\ndiff time:%ld\n",diff);
		Sleep(5*1000);
	
	}
}
int _tmain(int argc, _TCHAR* argv[])
{
		WORD sockVersion = MAKEWORD(2,2);
		WSADATA data; 
		if(WSAStartup(sockVersion, &data) != 0)
		{
			return 0;
		}

		sclient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
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
		char * sendData = "��ã�TCP����ˣ����ǿͻ���!\n";
		char sendbuf[BUFFER_SIZE];
		char recvbuf[BUFFER_SIZE];
		

			//��ʼ��������Ϣ
			char send_str[2048] = {0};
			
			// char api[]="http://221.12.156.123:16915/issue_tradeweb/httpXmlServlet";
			//char hostname[]="221.12.156.123:16915";

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
			
			strcat(send_str, "<?xml version=\"1.0\" encoding=\"gb2312\"?><GNNT><REQ name=\"logon\"><USER_ID>1299906727</USER_ID><PASSWORD>418331101</PASSWORD><REGISTER_WORD></REGISTER_WORD><VERSIONINFO></VERSIONINFO><LOGONTYPE>pc</LOGONTYPE></REQ></GNNT>");

			printf("!!!!!!!!!!11step!!!!!!!!!!!!!send_str:%s\n",send_str);
		send(sclient, send_str, strlen(send_str), 0);

		 recv(sclient, recvbuf, sizeof(recvbuf),0); ///����

		 char *start=strstr(recvbuf,"<RETCODE>");
		 char *end=strstr(recvbuf,"</RETCODE>");
		 memset(retcode, 0, sizeof(retcode));
		 memcpy(retcode,start+sizeof("<RETCODE>")-1,end-start-sizeof("<RETCODE>")+1);
		 printf("!!!!!!!!555!!!!!!!!!!!!!!!RETCODE:%s\n",retcode);
		 memset(sendbuf, 0, sizeof(sendbuf));
		 memset(recvbuf, 0, sizeof(recvbuf));
		 memset(send_str, 0, sizeof(send_str));

		 //ͷ��Ϣ
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

		 send(sclient, send_str, strlen(send_str),0); ///����
		 //if(strcmp(sendbuf,"exit\n")==0)
		 //  break;
		 recv(sclient, recvbuf, sizeof(recvbuf),0); ///����
		 fputs(recvbuf, stdout);

		 memset(sendbuf, 0, sizeof(sendbuf));
		 memset(recvbuf, 0, sizeof(recvbuf));
		 memset(send_str, 0, sizeof(send_str));

		 //ͷ��Ϣ
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

		 /*send(sclient, send_str, strlen(send_str),0); ///����

		 while(0!=recv(sclient, recvbuf, sizeof(recvbuf),0)) ///����
		 fputs(recvbuf, stdout);*/

		 

		 DWORD ThreadID;
		 HANDLE hThread;
		 hThread=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThreadFuncSycTime,NULL,0,&ThreadID);
		 char ybcode[10];
		 int   count=0;
		 char ybsale[10];
		 cout<<"please input a string:";
		 cout<<endl;
		  std::cin>>ybcode;
		  cout<<"ybcode:" <<ybcode<<endl;

		 while (1)
		 {
			
			 if (GetCurrMSForMe()>(GetSetTimeForMe()-diff))
			 {
				 printf("sussces!!!!!\n");
			 }
			 
		 }
		closesocket(sclient);
		WSACleanup();

	return 0;
}

