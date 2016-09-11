
// ybk2Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ybk2.h"
#include "ybk2Dlg.h"
#include "afxdialogex.h"
#include "ClientNet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// Cybk2Dlg �Ի���




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


// Cybk2Dlg ��Ϣ�������

BOOL Cybk2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void Cybk2Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR Cybk2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void Cybk2Dlg::OnCbnSelchangeCombo1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString accout;
	CString passwd;
	GetDlgItemText(IDC_EDIT_ACCOUT, accout);
	GetDlgItemText(IDC_EDIT_MIMA, passwd);
	//SetDlgItemText(IDC_EDIT_YU_E, accout);
	
	DWORD ThreadID;
	hThread=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThreadFunc,this,0,&ThreadID);

}
