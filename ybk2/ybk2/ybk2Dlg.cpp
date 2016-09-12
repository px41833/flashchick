
// ybk2Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ybk2.h"
#include "ybk2Dlg.h"
#include "afxdialogex.h"
#include "ClientNet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Cybk2Dlg 对话框




Cybk2Dlg::Cybk2Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(Cybk2Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cybk2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	
}

BEGIN_MESSAGE_MAP(Cybk2Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, &Cybk2Dlg::OnBnClickedButtonLogin)
END_MESSAGE_MAP()


// Cybk2Dlg 消息处理程序

BOOL Cybk2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void Cybk2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void Cybk2Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR Cybk2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void Cybk2Dlg::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
}

void ThreadFunc(LPVOID lpParameter)
{
	CTime time;
	CString strTime;
	char recdata[1024*4]={0};
	Cybk2Dlg *ybkdlg= (Cybk2Dlg *)lpParameter;
	CClientNet *pfconnect=new CClientNet();
	pfconnect->Connect(16915,"123.59.182.105");

	pfconnect->BuildXmlData_Logon(recdata,ybkdlg->GetDlgItemInt(IDC_EDIT_ACCOUT,NULL,TRUE),ybkdlg->GetDlgItemInt(IDC_EDIT_MIMA,NULL,TRUE));
	pfconnect->SendMsg(recdata,2048);
	memset(recdata,0,sizeof(recdata));
	//Sleep(1000);
	pfconnect->RecvMsg(recdata,sizeof(recdata));
	//AfxMessageBox(recdata);
	if(strlen(recdata)>0)
	{
	pfconnect->ProcXmlDateEx(recdata);
	}
	memset(recdata,0,sizeof(recdata));
	pfconnect->BuildXmlData_ReqFirmInfo(recdata);
	//Sleep(1000*2);
	pfconnect->SendMsg(recdata,2048);
	memset(recdata,0,sizeof(recdata));
	//Sleep(1000);
	pfconnect->RecvMsg(recdata,sizeof(recdata));
	AfxMessageBox(recdata);
	/*if(strlen(recdata)>0)
	{
		pfconnect->ProcXmlDate(recdata);
	}*/
	delete pfconnect;
	int m_bRun=TRUE;
	/*while(m_bRun)
	{
		time=CTime::GetCurrentTime();
		strTime=time.Format("%H:%M:%S");
		ybkdlg->SetDlgItemText(IDC_EDIT_YU_E, strTime);
		Sleep(1000);
	}*/
}
#pragma  comment(lib,"ws2_32.lib")
#define MYPORT  16915
#define IPADDR  "123.59.182.105"
#define BUFFER_SIZE 1024
int test()
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

		printf("!!!!!!!!!!444!!!!!!!!!!!!!send_str:%s\n",send_str);
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

		printf("!!!!!!!!!!77777!!!!!!!!!!!!!send_str:%s\n",send_str);

		send(sclient, send_str, strlen(send_str),0); ///发送
		//if(strcmp(sendbuf,"exit\n")==0)
		//  break;
		recv(sclient, recvbuf, sizeof(recvbuf),0); ///接收
		fputs(recvbuf, stdout);

		closesocket(sclient);
		WSACleanup();
	}
void Cybk2Dlg::OnBnClickedButtonLogin()
{
	// TODO: 在此添加控件通知处理程序代码
	CString accout;
	CString passwd;
	GetDlgItemText(IDC_EDIT_ACCOUT, accout);
	GetDlgItemText(IDC_EDIT_MIMA, passwd);
	//SetDlgItemText(IDC_EDIT_YU_E, accout);
	test();
	//DWORD ThreadID;
	//hThread=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThreadFunc,this,0,&ThreadID);
	
}
