#pragma once

#include "common.h"
#include <vector>
using namespace std;
class ClientNet
{
public:
	ClientNet(void);
	ClientNet(CString accout,CString passwd ,CString ipaddr,CString port,DWORD threadid);
	~ClientNet(void);
public:
	CString LoginAccout;
	CString LoginPasswd;
	CString ConIpAddr;
	CString ConPort;
	CString XmlHead;
	CString RetRandCode;

	SOCKET m_sock;
	DWORD MainWinThreadID;//
	DWORD ClientThreadID;
	BOOL  RunFlag;
	CString SvnMil;//交易所上服务器的毫秒数 
	vector<YB_PARAM> yb_vec;
	time_t time_localcurrent;
	time_t settimediff;
	//time_t autosynctimediff;
	BOOL ListCommit; //表单是否提交过的标志
	int cm_check;//是否选择自动算时间差值

public:
	int Connect(int port,const char* address);
	int Connect(void);
	//发送信息
	int SendMsg(const char* msg,int len);

	int RecvMsg(char *msg,int len);
	//关闭
	void Close();

	CString InitXmlData_Head();
	CString BuildXmlData_Logon(CString s,int len);
	CString BuildXmlData_GetSvnTime(CString s,int len);
	CString BuildXmlData_GetFirmInfo(CString s,int len);
	CString BuildXmlData_Holding(CString s,int len);   //刷新持仓
	CString BuildXmlData_DataQuery(CString s,int len,CString querycode);  

	int AddCommitList(YB_PARAM ybk);
	int SetCommitTime(COM_TIME cmtime);
	void StartCommitList(void);
	int GetCommitListLen(void);

	void RunTimeCommit(void);//同步服务器时间和提前订单

};

