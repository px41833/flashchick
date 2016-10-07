
// ybkDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ybk.h"
#include "ybkDlg.h"
#include "afxdialogex.h"
#include "ClientNet.h"
#include "common.h"
#include "addrlist.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


time_t GetCurrMSForMe() //��ȡ����1970�굽���ڵĺ�����
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

time_t GetSetTimeForMe(WORD day,WORD hour,WORD mintue,WORD second) //��ȡ�û�����ת�����1970�굽���ڵĺ�����
{
	SYSTEMTIME tTime = {0};
	//GetSystemTime(&tTime);
	GetLocalTime(&tTime);
	//printf("year:%d,month:%d,day:%d,hour:%d,minute:%d,sec:%d\n",tTime.wYear,tTime.wMonth,tTime.wDay,tTime.wHour,tTime.wMinute,tTime.wSecond);
	//tTime.wMonth=month;
	tTime.wDay=day;
	tTime.wHour=hour;
	tTime.wMinute=mintue;
	tTime.wSecond=second;
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
SYSTEMTIME GetSvnTimeToLocal(time_t aa) //�Դӷ�������ȡ����ʱ�����ת���ɱ���ʱ��
{
	SYSTEMTIME tTime = {0};
	ULARGE_INTEGER ui;
	ui.QuadPart=aa*10000+116444736000000000;

	 FILETIME fTime = {0};
	 fTime.dwLowDateTime=ui.LowPart;
	 fTime.dwHighDateTime=ui.HighPart;

	 FILETIME tempTime = {0};
	 //LocalFileTimeToFileTime(, );

	 FileTimeToLocalFileTime(&fTime,&tempTime);

	 FileTimeToSystemTime(&tempTime, &tTime); 
	 return tTime;
}
 void GetStrFromS1ToS2(const char *s,const char *start_s,const char *end_s,char *out)
{
	char parastr[128];
	sprintf_s(parastr,"%s",start_s);
const char *start=strstr(s,start_s);
const char *end=strstr(s,end_s);
memset(out, 0, sizeof(out));
//memcpy(out,strstr(s,start)+sizeof(start)-1,strstr(s,end)-strstr(s,start)-sizeof(start)+1);
TRACE("start:%s,%d,%d,%d\n",start_s,sizeof(start_s),strlen(parastr),sizeof("<TV_U>"));
memcpy(out,start+sizeof(start_s)-1,end-start-sizeof(start_s)+1);

//return out;
}
 CString GetStrFromS1ToS2Ex(const char *s,CString start_s,CString end_s)
 {
	
	 CString localstr(s);
	 int StartPos = localstr.Find(start_s);
	 int EndPos = localstr.Find(end_s);
	 //CString findstr=_T("<FN>");
	 CString sSubStr = localstr.Mid(StartPos+start_s.GetLength(),EndPos-StartPos-start_s.GetLength());
	
	 return sSubStr;
 }
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


// CybkDlg �Ի���




CybkDlg::CybkDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CybkDlg::IDD, pParent)
	, m_Row(0)
	, m_Col(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CybkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_BUY_SELL, m_BuySaleList);
	DDX_Control(pDX, IDC_LIST_PRODUCT, m_ProductList);
	DDX_Control(pDX, IDC_EDIT_LIST, m_edit);
}

BEGIN_MESSAGE_MAP(CybkDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CybkDlg::OnBnClickedButtonLogin)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_BUY_SELL, &CybkDlg::OnNMDblclkListBuySell)
	ON_EN_KILLFOCUS(IDC_EDIT_LIST, &CybkDlg::OnEnKillfocusEditList)
	ON_BN_CLICKED(IDC_BUTTON_START_COMMIT, &CybkDlg::OnBnClickedButtonStartCommit)
END_MESSAGE_MAP()


// CybkDlg ��Ϣ�������

BOOL CybkDlg::OnInitDialog()
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
	CRect rect;   
	//��ʼ����������Ʒ
	m_ProductList.GetClientRect(&rect);   


	//// Ϊ�б���ͼ�ؼ��������   
	m_ProductList.InsertColumn(0, _T("���"), LVCFMT_CENTER, rect.Width()/10, 0);   
	m_ProductList.InsertColumn(1, _T("��Ʒ����"), LVCFMT_CENTER, (rect.Width()/10)*3, 1);   
	m_ProductList.InsertColumn(2, _T("��Ʒ����"), LVCFMT_CENTER, rect.Width()/5, 2); 
	m_ProductList.InsertColumn(3, _T("��ͣ��"), LVCFMT_CENTER, rect.Width()/5, 3);   
	m_ProductList.InsertColumn(4, _T("��ͣ��"), LVCFMT_CENTER, rect.Width()/5, 4);   

	// Ϊ�б���ͼ�ؼ����ȫ��ѡ�к�դ����   
	m_ProductList.SetExtendedStyle(m_ProductList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES|LVS_EX_ONECLICKACTIVATE);   
    
	///////////////////////////////
	//��ʼ�������б�
	// ��ȡ��������б���ͼ�ؼ���λ�úʹ�С   
	m_BuySaleList.GetClientRect(&rect);   


	//// Ϊ�б���ͼ�ؼ��������   
	m_BuySaleList.InsertColumn(0, _T("���"), LVCFMT_CENTER, rect.Width()/12, 0);   
	m_BuySaleList.InsertColumn(1, _T("��Ʒ����"), LVCFMT_CENTER, rect.Width()/6, 1);   
	m_BuySaleList.InsertColumn(2, _T("��Ʒ����"), LVCFMT_CENTER, rect.Width()/4, 2); 
	m_BuySaleList.InsertColumn(3, _T("��/��"), LVCFMT_CENTER, rect.Width()/6, 3);   
	m_BuySaleList.InsertColumn(4, _T("�۸�"), LVCFMT_CENTER, rect.Width()/6, 4);   
	m_BuySaleList.InsertColumn(5, _T("����"), LVCFMT_CENTER, rect.Width()/6, 5); 
	// Ϊ�б���ͼ�ؼ����ȫ��ѡ�к�դ����   
	m_BuySaleList.SetExtendedStyle(m_BuySaleList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES|LVS_EX_ONECLICKACTIVATE);   

	//m_BuySaleList.SetItemCount(4);
	m_BuySaleList.InsertItem(0,_T("1"));
	m_BuySaleList.InsertItem(1,_T("2")); 
	m_BuySaleList.InsertItem(2,_T("3")); 
	m_BuySaleList.InsertItem(3,_T("4")); 
	m_BuySaleList.InsertItem(4,_T("5")); 
	m_BuySaleList.InsertItem(5,_T("6"));

	/////////////////////////��ʼ����ʱ��///////////////////////
	SYSTEMTIME tTime = {0};
	GetLocalTime(&tTime);
	SetDlgItemInt(IDC_EDIT_OPENING_DAY,tTime.wDay);
	SetDlgItemInt(IDC_EDIT_OPENING_HOUR,9);
	SetDlgItemInt(IDC_EDIT_OPENING_MINUT,30);
	SetDlgItemInt(IDC_EDIT_OPENING_SECOND,0);
	SetDlgItemInt(IDC_EDIT_OPENING_MIL,0);

	//�Ա�����ʼ��
	 time_localcurrent=0;
	 settimediff=0;
	 autosynctimediff=0;
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CybkDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CybkDlg::OnPaint()
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
HCURSOR CybkDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
void ThreadFuncSyncCommit(LPVOID lpParameter)
{
   ClientNet *ybkclient= (ClientNet *)lpParameter;
   CString logon_s;
   ybkclient->BuildXmlData_GetFirmInfo(logon_s,0);
   DWORD tick=0,tick1=0;
   time_t tick_t=0;
   CString synctime;
   while(ybkclient->RunFlag)
   {
	   tick=GetTickCount();
	  //time_t  tick_t=time();
	  // Sleep(4*500);
	   if(tick!=tick1)
	   { 
		   if (tick%3000==0)
		   {
			   TRACE("test run\n");
			   ybkclient->BuildXmlData_GetSvnTime(synctime,0);
		   }
		   tick1=tick;
	   }
	   
		//ybkclient->RunTimeCommit();
   }
   
}
BOOL MByteToWChar(LPCSTR lpcszStr, LPWSTR lpwszStr, DWORD dwSize)
{
	// Get the required size of the buffer that receives the Unicode 
	// string. 
	DWORD dwMinSize;
	dwMinSize = MultiByteToWideChar (CP_ACP, 0, lpcszStr, -1, NULL, 0);

	if(dwSize < dwMinSize)
	{
		return FALSE;
	}


	// Convert headers from ASCII to Unicode.
	MultiByteToWideChar (CP_ACP, 0, lpcszStr, -1, lpwszStr, dwMinSize);  
	return TRUE;
}
void ThreadFunc(LPVOID lpParameter)
{
	CTime time;
	CString strTime;
	char recdata[1024*4]={0};
	CybkDlg *ybkdlg= (CybkDlg *)lpParameter;
	CString paccout,ppasswd;
	ybkdlg->GetDlgItemText(IDC_EDIT_ACCOUT,paccout);
	ybkdlg->GetDlgItemText(IDC_EDIT_PASSWD,ppasswd);
	ClientNet *pfconnect=new ClientNet(paccout,ppasswd,ExchangePara[0][0],ExchangePara[0][1],ybkdlg->MainThreadID);
	
	//1.init connect
	pfconnect->Connect();
	//2.��¼��ȡ������
	CString logon_s;
	pfconnect->BuildXmlData_Logon(logon_s,0);
	MSG msg;
	//while(1)
	{

	
	while(GetMessage(&msg,0,0,0))
	{
		if(msg.message == MESSAGE_LOGON_SUCCESS)
		{
			TRACE("message:%s\n",(char *)msg.wParam);
			char *procstr=(char *)msg.wParam;
			CString sSubStr =GetStrFromS1ToS2Ex(procstr,_T("<RETCODE>"),_T("</RETCODE>"));
			if (_ttoi(sSubStr)>=0)
			{
				CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThreadFuncSyncCommit,pfconnect,0,NULL);
			}
			else
			{
				sSubStr.Empty();
				 sSubStr =GetStrFromS1ToS2Ex(procstr,_T("<MESSAGE>"),_T("</MESSAGE>"));
                MessageBox(NULL,sSubStr,_T("�޷�����"),MB_OK);
				pfconnect->RunFlag=FALSE;
				delete pfconnect;
				break;
				
			}
			
			//pfconnect->RunTimeCommit();
		}
		else if(msg.message == MESSAGE_SYNC_TIME)
		{
			//ͬ��ʱ�䣬ˢ�¿ؼ�
			TRACE("message time:%s\n",(char *)msg.wParam);
			char *procstr=(char *)msg.wParam;
			CString sSubStr =GetStrFromS1ToS2Ex(procstr,_T("<RETCODE>"),_T("</RETCODE>"));
			if (_ttoi(sSubStr)>=0)
			{
				sSubStr.Empty();
				sSubStr =GetStrFromS1ToS2Ex(procstr,_T("<TV_U>"),_T("</TV_U>"));
				
				LONGLONG timea=0,local_t=GetCurrMSForMe();
				//sscanf_s((char *)msg.wParam,"%lld",&timea);
				USES_CONVERSION;
				sscanf_s((const char*)T2A(sSubStr),"%lld",&timea);
			    if(ybkdlg->autosynctimediff==0)
				ybkdlg->autosynctimediff=local_t-timea;
				else
                ybkdlg->autosynctimediff=(local_t-timea)>ybkdlg->autosynctimediff?ybkdlg->autosynctimediff:(local_t-timea);
			//char timechuo[128];
			//char startstr[]="<TV_U>";
			TRACE("ss:%d,%d\n",ybkdlg->autosynctimediff,(local_t-timea));
			//GetStrFromS1ToS2((char *)msg.wParam,startstr,"</TV_U>",timechuo);
			//LONGLONG timea=0;
			////sscanf_s((char *)msg.wParam,"%lld",&timea);
			//sscanf_s(timechuo,"%lld",&timea);
			
			SYSTEMTIME stLocal= {0};
			CString strTime;
			stLocal=GetSvnTimeToLocal(timea);
			strTime.Format(_T("%2d/%d/%d %d:%d:%d:%d"),stLocal.wYear,stLocal.wMonth,stLocal.wDay,stLocal.wHour,stLocal.wMinute,stLocal.wSecond,stLocal.wMilliseconds);
			SetDlgItemText(ybkdlg->m_hWnd,IDC_EDIT_SERVER_TIME,strTime);

			SYSTEMTIME tTime = {0};
			CString localTime;
			GetLocalTime(&tTime);
			localTime.Format(_T("%2d/%d/%d %d:%d:%d:%d"),tTime.wYear,tTime.wMonth,tTime.wDay,tTime.wHour,tTime.wMinute,tTime.wSecond,tTime.wMilliseconds);
			SetDlgItemText(ybkdlg->m_hWnd,IDC_EDIT_LOCAL_TIME,localTime);
			}
			else
			{
				TRACE("5555555\n");
				sSubStr.Empty();
				sSubStr =GetStrFromS1ToS2Ex(procstr,_T("<MESSAGE>"),_T("</MESSAGE>"));
				MessageBox(NULL,sSubStr,_T("����"),MB_OK);
				pfconnect->RunFlag=FALSE;
				delete pfconnect;
				break;
			}
		}
		else if(msg.message == MESSAGE_FIRM_INFO)
		{
			TRACE("MESSAGE_FIRM_INFO:%s\n",(char *)msg.wParam);
			char *procstr=(char *)msg.wParam;
			//char outstr[5];
			
			//memcpy(outstr,strstr(procstr,"<FN>")+sizeof("<FN>")-1,strstr(procstr,"</FN>")-strstr(procstr,"<FN>")-sizeof("<FN>")+1);
			//localstr=outstr;
			/*CString localstr(procstr);
			int StartPos = localstr.Find(_T("<FN>"));
			int EndPos = localstr.Find(_T("</FN>"));
			CString findstr=_T("<FN>");
			CString sSubStr = localstr.Mid(StartPos+findstr.GetLength(),EndPos-StartPos-findstr.GetLength());*/
			CString sSubStr =GetStrFromS1ToS2Ex(procstr,_T("<FN>"),_T("</FN>"));
			SetDlgItemText(ybkdlg->m_hWnd,IDC_EDIT_USER,sSubStr);
			CString  balance=GetStrFromS1ToS2Ex(procstr,_T("<JYSQY>"),_T("</JYSQY>"));
			SetDlgItemText(ybkdlg->m_hWnd,IDC_EDIT_BALANCE,balance);
		}
		TRACE("1111111111111\n");
	}
	TRACE("2222222222222222\n");
	}
	TRACE("3333333333\n");
	//3.����ͬ���������߳�
	//4.�������շ������ݴ����߳�
	//5.ѭ���ȴ�ʱ��ȥ�ύ����

}
//DWORD ThreadMsgProc(LPVOID lParam)
//{
//	MSG msg;
//	while(GetMessage(&msg,0,0,0))
//	{
//		if(msg.message == TALK_MESSAGE)
//		{
//			//MessageBox(NULL,L"Hi",L"Worker Thread",MB_OK);
//		}
//		//DispatchMessage(&msg);
//	}
//	//MessageBox(NULL,L"Thread will close by pressing OK",L"From Worker Thread",MB_OK);
//	//AfxGetApp()->m_pMainWnd->PostMessageW(TALK_MESSAGE+1,0,0);
//	return TRUE;
//}
void CybkDlg::OnBnClickedButtonLogin()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	WORD sockVersion = MAKEWORD(2,2);
	WSADATA data; 
	if(WSAStartup(sockVersion, &data) != 0)
	{
		return ;
	}
	
	CString accout;
	CString passwd;
	GetDlgItemText(IDC_EDIT_ACCOUT, accout);
	GetDlgItemText(IDC_EDIT_PASSWD, passwd);
	
	/*hThread=*/CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThreadFunc,this,0,&this->MainThreadID);
	//ExitThread()
	//	TerminateThread 
		//Destroy
	//CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThreadMsgProc,0,0,&m_dwThread);
	
}


void CybkDlg::OnNMDblclkListBuySell(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	NM_LISTVIEW* pNMListView=(NM_LISTVIEW*)pNMHDR;
	CRect rc;
	m_Row = pNMListView->iItem;//���ѡ�е���
	m_Col = pNMListView->iSubItem;//���ѡ����
	//CEdit m_edit;

	if   (pNMListView->iSubItem != 0) //���ѡ���������;
	{
		m_BuySaleList.GetSubItemRect(m_Row,m_Col,LVIR_LABEL,rc);//��������RECT��
		m_edit.SetParent(&m_BuySaleList);//ת������Ϊ�б���е�����
		m_edit.MoveWindow(rc);//�ƶ�Edit��RECT���ڵ�λ��;
		m_edit.SetWindowText(m_BuySaleList.GetItemText(m_Row,m_Col));//���������е�ֵ����Edit�ؼ��У�
		m_edit.ShowWindow(SW_SHOW);//��ʾEdit�ؼ���
		m_edit.SetFocus();//����Edit����
		m_edit.ShowCaret();//��ʾ���
		m_edit.SetSel(-1);//������ƶ������


	}
	*pResult = 0;
}


void CybkDlg::OnEnKillfocusEditList()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str;
	if(m_edit.m_hWnd!=NULL)
	{
		m_edit.GetWindowText(str);//ȡ�ñ༭�������
		m_BuySaleList.SetItemText(m_Row,m_Col,str);//�������ݸ��µ�CListCtrl��
		m_edit.ShowWindow(SW_HIDE);//���ر༭��
	}
}


void CybkDlg::OnBnClickedButtonStartCommit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	UINT m_day=GetDlgItemInt(IDC_EDIT_OPENING_DAY,NULL,FALSE);
	UINT m_hour=GetDlgItemInt(IDC_EDIT_OPENING_HOUR,NULL,FALSE);
	UINT m_minute=GetDlgItemInt(IDC_EDIT_OPENING_MINUT,NULL,FALSE);
	UINT m_second=GetDlgItemInt(IDC_EDIT_OPENING_SECOND,NULL,FALSE);
	settimediff=GetDlgItemInt(IDC_EDIT_OPENING_MIL,NULL,TRUE);
	LONGLONG timediff=67985;
	TRACE("diff:%lld\n",timediff+settimediff);
	//LONGLONG timediff=GetDlgItemInt(IDC_EDIT_OPENING_MIL,NULL,TRUE);
	time_localcurrent=GetSetTimeForMe(m_day,m_hour,m_minute,m_second);
}
