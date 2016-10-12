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
	CString SvnMil;//�������Ϸ������ĺ����� 
	vector<YB_PARAM> yb_vec;
	time_t time_localcurrent;
	int settimediff;
	time_t autosynctimediff;
	BOOL ListCommit; //���Ƿ��ύ���ı�־

public:
	int Connect(int port,const char* address);
	int Connect(void);
	//������Ϣ
	int SendMsg(const char* msg,int len);

	int RecvMsg(char *msg,int len);
	//�ر�
	void Close();

	CString InitXmlData_Head();
	CString BuildXmlData_Logon(CString s,int len);
	CString BuildXmlData_GetSvnTime(CString s,int len);
	CString BuildXmlData_GetFirmInfo(CString s,int len);
	int AddCommitList(YB_PARAM ybk);
	
	void RunTimeCommit(void);//ͬ��������ʱ�����ǰ����

};

