
// ybkDlg.cpp : 实现文件
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


time_t GetCurrMSForMe() //获取本机1970年到现在的毫秒数
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

time_t GetSetTimeForMe(WORD day,WORD hour,WORD mintue,WORD second) //获取用户设置转化后的1970年到现在的毫秒数
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
SYSTEMTIME GetSvnTimeToLocal(time_t aa) //对从服务器获取到的时间进行转化成本地时间
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


// CybkDlg 对话框




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


// CybkDlg 消息处理程序

BOOL CybkDlg::OnInitDialog()
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
	CRect rect;   
	//初始化交易所藏品
	m_ProductList.GetClientRect(&rect);   


	//// 为列表视图控件添加三列   
	m_ProductList.InsertColumn(0, _T("序号"), LVCFMT_CENTER, rect.Width()/10, 0);   
	m_ProductList.InsertColumn(1, _T("藏品代码"), LVCFMT_CENTER, (rect.Width()/10)*3, 1);   
	m_ProductList.InsertColumn(2, _T("藏品名称"), LVCFMT_CENTER, rect.Width()/5, 2); 
	m_ProductList.InsertColumn(3, _T("涨停价"), LVCFMT_CENTER, rect.Width()/5, 3);   
	m_ProductList.InsertColumn(4, _T("跌停价"), LVCFMT_CENTER, rect.Width()/5, 4);   

	// 为列表视图控件添加全行选中和栅格风格   
	m_ProductList.SetExtendedStyle(m_ProductList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES|LVS_EX_ONECLICKACTIVATE);   
    
	///////////////////////////////
	//初始化买卖列表
	// 获取编程语言列表视图控件的位置和大小   
	m_BuySaleList.GetClientRect(&rect);   


	//// 为列表视图控件添加三列   
	m_BuySaleList.InsertColumn(0, _T("序号"), LVCFMT_CENTER, rect.Width()/12, 0);   
	m_BuySaleList.InsertColumn(1, _T("藏品代码"), LVCFMT_CENTER, rect.Width()/6, 1);   
	m_BuySaleList.InsertColumn(2, _T("藏品名称"), LVCFMT_CENTER, rect.Width()/4, 2); 
	m_BuySaleList.InsertColumn(3, _T("买/卖"), LVCFMT_CENTER, rect.Width()/6, 3);   
	m_BuySaleList.InsertColumn(4, _T("价格"), LVCFMT_CENTER, rect.Width()/6, 4);   
	m_BuySaleList.InsertColumn(5, _T("数量"), LVCFMT_CENTER, rect.Width()/6, 5); 
	// 为列表视图控件添加全行选中和栅格风格   
	m_BuySaleList.SetExtendedStyle(m_BuySaleList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES|LVS_EX_ONECLICKACTIVATE);   

	//m_BuySaleList.SetItemCount(4);
	m_BuySaleList.InsertItem(0,_T("1"));
	m_BuySaleList.InsertItem(1,_T("2")); 
	m_BuySaleList.InsertItem(2,_T("3")); 
	m_BuySaleList.InsertItem(3,_T("4")); 
	m_BuySaleList.InsertItem(4,_T("5")); 
	m_BuySaleList.InsertItem(5,_T("6"));

	/////////////////////////初始化打单时间///////////////////////
	SYSTEMTIME tTime = {0};
	GetLocalTime(&tTime);
	SetDlgItemInt(IDC_EDIT_OPENING_DAY,tTime.wDay);
	SetDlgItemInt(IDC_EDIT_OPENING_HOUR,9);
	SetDlgItemInt(IDC_EDIT_OPENING_MINUT,30);
	SetDlgItemInt(IDC_EDIT_OPENING_SECOND,0);
	SetDlgItemInt(IDC_EDIT_OPENING_MIL,0);

	//对变量初始化
	 time_localcurrent=0;
	 settimediff=0;
	 autosynctimediff=0;
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CybkDlg::OnPaint()
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
	//2.登录获取返回码
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
                MessageBox(NULL,sSubStr,_T("无法连接"),MB_OK);
				pfconnect->RunFlag=FALSE;
				delete pfconnect;
				break;
				
			}
			
			//pfconnect->RunTimeCommit();
		}
		else if(msg.message == MESSAGE_SYNC_TIME)
		{
			//同步时间，刷新控件
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
				MessageBox(NULL,sSubStr,_T("错误"),MB_OK);
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
	//3.创建同步服务器线程
	//4.创建接收返回数据处理线程
	//5.循环等待时间去提交定单

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
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加控件通知处理程序代码
	NM_LISTVIEW* pNMListView=(NM_LISTVIEW*)pNMHDR;
	CRect rc;
	m_Row = pNMListView->iItem;//获得选中的行
	m_Col = pNMListView->iSubItem;//获得选中列
	//CEdit m_edit;

	if   (pNMListView->iSubItem != 0) //如果选择的是子项;
	{
		m_BuySaleList.GetSubItemRect(m_Row,m_Col,LVIR_LABEL,rc);//获得子项的RECT；
		m_edit.SetParent(&m_BuySaleList);//转换坐标为列表框中的坐标
		m_edit.MoveWindow(rc);//移动Edit到RECT坐在的位置;
		m_edit.SetWindowText(m_BuySaleList.GetItemText(m_Row,m_Col));//将该子项中的值放在Edit控件中；
		m_edit.ShowWindow(SW_SHOW);//显示Edit控件；
		m_edit.SetFocus();//设置Edit焦点
		m_edit.ShowCaret();//显示光标
		m_edit.SetSel(-1);//将光标移动到最后


	}
	*pResult = 0;
}


void CybkDlg::OnEnKillfocusEditList()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	if(m_edit.m_hWnd!=NULL)
	{
		m_edit.GetWindowText(str);//取得编辑框的内容
		m_BuySaleList.SetItemText(m_Row,m_Col,str);//将该内容更新到CListCtrl中
		m_edit.ShowWindow(SW_HIDE);//隐藏编辑框
	}
}


void CybkDlg::OnBnClickedButtonStartCommit()
{
	// TODO: 在此添加控件通知处理程序代码

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
