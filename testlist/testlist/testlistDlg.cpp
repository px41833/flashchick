
// testlistDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "testlist.h"
#include "testlistDlg.h"
#include "afxdialogex.h"

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


// CtestlistDlg 对话框




CtestlistDlg::CtestlistDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CtestlistDlg::IDD, pParent)
	, m_Row(0)
	, m_Col(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CtestlistDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRAM_LANG_LIST, m_programLangList);
	DDX_Control(pDX, IDC_EDIT1, m_edit);
}

BEGIN_MESSAGE_MAP(CtestlistDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(NM_CLICK, IDC_PROGRAM_LANG_LIST, &CtestlistDlg::OnNMClickProgramLangList)
	ON_NOTIFY(NM_DBLCLK, IDC_PROGRAM_LANG_LIST, &CtestlistDlg::OnNMDblclkProgramLangList)
	ON_EN_KILLFOCUS(IDC_EDIT1, &CtestlistDlg::OnEnKillfocusEdit1)
END_MESSAGE_MAP()


// CtestlistDlg 消息处理程序

BOOL CtestlistDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

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


	CRect rect;   

	// 获取编程语言列表视图控件的位置和大小   
	m_programLangList.GetClientRect(&rect);   

	
	//// 为列表视图控件添加三列   
	m_programLangList.InsertColumn(0, _T("语言"), LVCFMT_CENTER, rect.Width()/3, 0);   
	m_programLangList.InsertColumn(1, _T("2012.02排名"), LVCFMT_CENTER, rect.Width()/3, 1);   
	m_programLangList.InsertColumn(2, _T("2011.02排名"), LVCFMT_CENTER, rect.Width()/3, 2); 
	// 为列表视图控件添加全行选中和栅格风格   
	m_programLangList.SetExtendedStyle(m_programLangList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES|LVS_EX_ONECLICKACTIVATE);   

	m_programLangList.SetItemCount(4);

	// 在列表视图控件中插入列表项，并设置列表子项文本   
	/*m_programLangList.InsertItem(0, _T("Java"));   
	m_programLangList.SetItemText(0, 1, _T("1"));   
	m_programLangList.SetItemText(0, 2, _T("1"));   
	m_programLangList.InsertItem(1, _T("C"));   
	m_programLangList.SetItemText(1, 1, _T("2"));   
	m_programLangList.SetItemText(1, 2, _T("2"));   
	m_programLangList.InsertItem(2, _T("C#"));   
	m_programLangList.SetItemText(2, 1, _T("3"));   
	m_programLangList.SetItemText(2, 2, _T("6"));   
	m_programLangList.InsertItem(3, _T("C++"));   
	m_programLangList.SetItemText(3, 1, _T("4"));   
	m_programLangList.SetItemText(3, 2, _T("3"));*/
	/*m_programLangList.InsertItem(0,_T(""));
	m_programLangList.InsertItem(1,_T("")); 
	m_programLangList.InsertItem(2,_T("")); 
	m_programLangList.InsertItem(3,_T("")); 
	m_programLangList.InsertItem(4,_T("")); 
	m_programLangList.InsertItem(5,_T(""));*/ 

	m_programLangList.OnDeleteItem()

	ShowWindow(SW_MAXIMIZE);

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CtestlistDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CtestlistDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CtestlistDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CtestlistDlg::OnNMClickProgramLangList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	

	NMLISTVIEW *pNMListView = (NMLISTVIEW*)pNMHDR;   

	if (-1 != pNMListView->iItem)        // 如果iItem不是-1，就说明有列表项被选择   
	{   
		;//m_programLangList.DeleteItem(pNMListView->iItem);
	}   
	*pResult = 0;
}


void CtestlistDlg::OnNMDblclkProgramLangList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	NM_LISTVIEW* pNMListView=(NM_LISTVIEW*)pNMHDR;
	CRect rc;
	  m_Row = pNMListView->iItem;//获得选中的行
	 m_Col = pNMListView->iSubItem;//获得选中列
	//CEdit m_edit;

	if   (pNMListView->iSubItem != 0) //如果选择的是子项;
	{
		m_programLangList.GetSubItemRect(m_Row,m_Col,LVIR_LABEL,rc);//获得子项的RECT；
		m_edit.SetParent(&m_programLangList);//转换坐标为列表框中的坐标
		m_edit.MoveWindow(rc);//移动Edit到RECT坐在的位置;
		m_edit.SetWindowText(m_programLangList.GetItemText(m_Row,m_Col));//将该子项中的值放在Edit控件中；
		m_edit.ShowWindow(SW_SHOW);//显示Edit控件；
		m_edit.SetFocus();//设置Edit焦点
		m_edit.ShowCaret();//显示光标
		m_edit.SetSel(-1);//将光标移动到最后


	}

	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


void CtestlistDlg::OnEnKillfocusEdit1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	if(m_edit.m_hWnd!=NULL)
	{
		m_edit.GetWindowText(str);//取得编辑框的内容
		m_programLangList.SetItemText(m_Row,m_Col,str);//将该内容更新到CListCtrl中
		m_edit.ShowWindow(SW_HIDE);//隐藏编辑框
	}
}
