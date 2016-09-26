// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <WINSOCK2.H>
#include <STDIO.H>
#include  <iostream>
#pragma  comment(lib,"ws2_32.lib")
#define MYPORT  10005
#define IPADDR  "43.240.136.232"
#define BUFFER_SIZE 1024
SOCKET sclient;
char api[]="/issue_tradeweb/httpXmlServlet ";
char hostname[]="43.240.136.232:10005";
char retcode[128];

char sertime[128];
long diff=0;
using namespace std;

int month;
int day;
int hour;
int mintue;
//int seccond;
 HANDLE hThread;

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

time_t GetSetTimeForMe() //获取1970年到现在的毫秒数
{
	SYSTEMTIME tTime = {0};
	//GetSystemTime(&tTime);
	GetLocalTime(&tTime);
	//printf("year:%d,month:%d,day:%d,hour:%d,minute:%d,sec:%d\n",tTime.wYear,tTime.wMonth,tTime.wDay,tTime.wHour,tTime.wMinute,tTime.wSecond);
	tTime.wMonth=month;
	tTime.wDay=day;
	tTime.wHour=hour;
	tTime.wMinute=mintue;
	tTime.wSecond=0;
	tTime.wMilliseconds=0;

	FILETIME fTime = {0};
	FILETIME tempTime = {0};
	SystemTimeToFileTime(&tTime, &tempTime);

	LocalFileTimeToFileTime(&tempTime, &fTime);

	ULARGE_INTEGER ui;
	ui.LowPart = fTime.dwLowDateTime;
	ui.HighPart = fTime.dwHighDateTime;

	return ((LONGLONG)(ui.QuadPart-116444736000000000)/10000);
}
void ThreadFuncSycTime(LPVOID lpParameter)
{
	char sendbuf[BUFFER_SIZE];
	char recvbuf[BUFFER_SIZE];


	//初始化发送信息
	char send_str[2048] = {0};
	memset(sendbuf, 0, sizeof(sendbuf));
	memset(recvbuf, 0, sizeof(recvbuf));
	memset(send_str, 0, sizeof(send_str));

	//头信息
	strcat(send_str, "POST ");
	strcat(send_str, api);
	strcat(send_str, " HTTP/1.1\r\n");
	strcat(send_str, "Content-Type: application/x-www-form-urlencoded\r\n");
	strcat(send_str, "Host: ");
	strcat(send_str, hostname);
	strcat(send_str, "\r\n");
	strcat(send_str, "Content-Length:216\r\n");
	//strcat(send_str, "Expect: 100-continue\r\n");
	strcat(send_str, "Connection: Keep-Alive\r\n\r\n");

	strcat(send_str, "<?xml version=\"1.0\" encoding=\"gb2312\"?><GNNT><REQ name=\"sys_time_query\"><USER_ID>01800000000154</USER_ID><LAST_ID>0</LAST_ID><TD_CNT>0</TD_CNT><SESSION_ID>");
	strcat(send_str,retcode);
	strcat(send_str,"</SESSION_ID><CU_LG>1</CU_LG></REQ></GNNT>");

	printf("!!!!!!!!!!4 step syn time!!!!!!!!!!!!!send_str:%s\n",send_str);
	SYSTEMTIME st;

	// CString strDate,strTime;
	while(1)
	{

		send(sclient, send_str, strlen(send_str),0); ///发送

		recv(sclient, recvbuf, sizeof(recvbuf),0); ///接收
		fputs(recvbuf, stdout);

		char *start=strstr(recvbuf,"<TV_U>");
		char *end=strstr(recvbuf,"</TV_U>");
		memset(sertime, 0, sizeof(sertime));
		memcpy(sertime,start+sizeof("<TV_U>")-1,end-start-sizeof("<TV_U>")+1);
		printf("\nsertime:%s\n",sertime);
		LONGLONG timea;
		sscanf(sertime,"%lld",&timea);
		//printf("\nsertimelong:%lld\n",timea);
		diff=GetCurrMSForMe()-timea;
		printf("\r\n settime:%lld\n",GetSetTimeForMe());
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
		cout<<"请输入购买的时间:月 日 时 分";
		cout<<endl;
		std::cin>>month>>day>>hour>>mintue;
		cout<<"time:" <<month<<' '<<day<<' '<<hour<<' '<<mintue<<endl;

		/*char ybcode[10];
		int   count=0;
		char ybsale[10];
		cout<<"请输入购买的邮票:代码 数量 价格";
		cout<<endl;
		std::cin>>ybcode>>count>>ybsale;
		cout<<"ybcode:" <<ybcode<<' '<<count<<' '<<ybsale<<endl;*/

		sclient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if(sclient == INVALID_SOCKET)
		{
			printf("invalid socket !");
			return 0;
		}

		sockaddr_in serAddr;
		serAddr.sin_family = AF_INET;
		serAddr.sin_port = htons(MYPORT);
		serAddr.sin_addr.S_un.S_addr = inet_addr(IPADDR); 
		if (connect(sclient, (sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
		{
			printf("connect error !");
			closesocket(sclient);
			return 0;
		}
		
		char sendbuf[BUFFER_SIZE];
		char recvbuf[BUFFER_SIZE];
		

			//初始化发送信息
			char send_str[2048] = {0};
		
			//头信息
			strcat(send_str, "POST ");
			strcat(send_str, api);
			strcat(send_str, " HTTP/1.1\r\n");
			strcat(send_str, "Content-Type: application/x-www-form-urlencoded\r\n");
			strcat(send_str, "Host: ");
			strcat(send_str, hostname);
			strcat(send_str, "\r\n");
			strcat(send_str, "Content-Length:271\r\n");
			//strcat(send_str, "Expect: 100-continue\r\n");
			strcat(send_str, "Connection: Keep-Alive\r\n\r\n");
			

			
			strcat(send_str, "<?xml version=\"1.0\" encoding=\"gb2312\"?><GNNT><REQ name=\"logon\"><USER_ID>01800000000154</USER_ID><PASSWORD>418331101</PASSWORD><REGISTER_WORD>201609252116332730180000000015458007.965574113085</REGISTER_WORD><VERSIONINFO></VERSIONINFO><LOGONTYPE>pc</LOGONTYPE></REQ></GNNT>");

			printf("logon send_str:%s\n",send_str);
		send(sclient, send_str, strlen(send_str), 0);

		 recv(sclient, recvbuf, sizeof(recvbuf),0); ///接收
		 //fputs(recvbuf, stdout);
		 //while(1);
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
		 strcat(send_str, "Content-Type: application/x-www-form-urlencoded\r\n");
		 strcat(send_str, "Host: ");
		 strcat(send_str, hostname);
		 strcat(send_str, "\r\n");
		 strcat(send_str, "Content-Length:157\r\n");
		 //strcat(send_str, "Expect: 100-continue\r\n");
		 strcat(send_str, "Connection: Keep-Alive\r\n\r\n");

		 strcat(send_str, "<?xml version=\"1.0\" encoding=\"gb2312\"?><GNNT><REQ name=\"firm_info\"><USER_ID>01800000000154</USER_ID><SESSION_ID>");
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

		 ////头信息
		 //strcat(send_str, "POST ");
		 //strcat(send_str, api);
		 //strcat(send_str, " HTTP/1.1\r\n");
		 //strcat(send_str, "Host: ");
		 //strcat(send_str, hostname);
		 //strcat(send_str, "\r\n");
		 //strcat(send_str, "Accept-Encoding:identity\r\n");
		 //strcat(send_str, "Content-Length:188\r\n\r\n");

		 //strcat(send_str, "<?xml version=\"1.0\" encoding=\"gb2312\"?><GNNT><REQ name=\"commodity_query\"><USER_ID>1299906727</USER_ID><COMMODITY_ID></COMMODITY_ID><SESSION_ID>");
		 //strcat(send_str,retcode);
		 //strcat(send_str,"</SESSION_ID></REQ></GNNT>");

		 //printf("!!!!!!!!!!3 step!!!!!!!!!!!!!send_str:%s\n",send_str);

		 /*send(sclient, send_str, strlen(send_str),0); ///发送

		 while(0!=recv(sclient, recvbuf, sizeof(recvbuf),0)) ///接收
		 fputs(recvbuf, stdout);*/
		 
		
		// while (1);
		 DWORD ThreadID;
		
		 hThread=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThreadFuncSycTime,NULL,0,&ThreadID);
		 //CloseHandle(hThread);

		 memset(sendbuf, 0, sizeof(sendbuf));
		 memset(recvbuf, 0, sizeof(recvbuf));
		 memset(send_str, 0, sizeof(send_str));

		 //头信息
		 strcat(send_str, "POST ");
		 strcat(send_str, api);
		 strcat(send_str, " HTTP/1.1\r\n");
		 strcat(send_str, "Content-Type: application/x-www-form-urlencoded\r\n");
		 strcat(send_str, "Host: ");
		 strcat(send_str, hostname);
		 strcat(send_str, "\r\n");
		 strcat(send_str, "Content-Length:405\r\n");
		 //strcat(send_str, "Expect: 100-continue\r\n");
		 strcat(send_str, "Connection: Keep-Alive\r\n\r\n");

		 strcat(send_str, "<?xml version=\"1.0\" encoding=\"gb2312\"?><GNNT><REQ name=\"order\"><USER_ID>01800000000154</USER_ID><CUSTOMER_ID>0180000000015400</CUSTOMER_ID><BUY_SELL>1</BUY_SELL><COMMODITY_ID>99202001</COMMODITY_ID><PRICE>1.57</PRICE><QTY>2</QTY><SETTLE_BASIS>1</SETTLE_BASIS><CLOSEMODE>0</CLOSEMODE><TIMEFLAG>0</TIMEFLAG><L_PRICE>0</L_PRICE><SESSION_ID>");
		 strcat(send_str,retcode);
		 strcat(send_str,"</SESSION_ID><BILLTYPE>0</BILLTYPE></REQ></GNNT>");
		 printf("buy send_str:%s\n",send_str);

		 while (1)
		 {
			
			 if (GetCurrMSForMe()>(GetSetTimeForMe()+diff))
			 {
				 
				 for (int i=0;i<2;i++)
				 {
					 printf("sussces:%lld\n",(GetSetTimeForMe()+diff));
					 send(sclient, send_str, strlen(send_str),0); ///发送
					 //if(strcmp(sendbuf,"exit\n")==0)
					 //  break;
					 recv(sclient, recvbuf, sizeof(recvbuf),0); ///接收
					 fputs(recvbuf, stdout);
				 }
				 //break;
				 TerminateThread(hThread,0);
				 while (1);
				 
			 }
			 
		 }
		closesocket(sclient);
		WSACleanup();

	return 0;
}

