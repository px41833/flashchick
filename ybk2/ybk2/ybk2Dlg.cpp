
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
	pfconnect->SendMsg(recdata,strlen(recdata)+252);
	memset(recdata,0,sizeof(recdata));
	//Sleep(1000);
	pfconnect->RecvMsg(recdata,sizeof(recdata));
	//AfxMessageBox(recdata);
	if(strlen(recdata)>0)
	{
	pfconnect->ProcXmlDate(recdata);
	}
	memset(recdata,0,sizeof(recdata));
	pfconnect->BuildXmlData_ReqFirmInfo(recdata);

	pfconnect->SendMsg(recdata,strlen(recdata)+153);
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
void Cybk2Dlg::OnBnClickedButtonLogin()
{
	// TODO: 在此添加控件通知处理程序代码
	CString accout;
	CString passwd;
	GetDlgItemText(IDC_EDIT_ACCOUT, accout);
	GetDlgItemText(IDC_EDIT_MIMA, passwd);
	//SetDlgItemText(IDC_EDIT_YU_E, accout);
	
	DWORD ThreadID;
	hThread=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThreadFunc,this,0,&ThreadID);

}
