#pragma once



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

	void RunTimeCommit(void);//ͬ��������ʱ�����ǰ����

};

