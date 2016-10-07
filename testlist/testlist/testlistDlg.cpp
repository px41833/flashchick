
// testlistDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "testlist.h"
#include "testlistDlg.h"
#include "afxdialogex.h"

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


// CtestlistDlg �Ի���




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


// CtestlistDlg ��Ϣ�������

BOOL CtestlistDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

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


	CRect rect;   

	// ��ȡ��������б���ͼ�ؼ���λ�úʹ�С   
	m_programLangList.GetClientRect(&rect);   

	
	//// Ϊ�б���ͼ�ؼ��������   
	m_programLangList.InsertColumn(0, _T("����"), LVCFMT_CENTER, rect.Width()/3, 0);   
	m_programLangList.InsertColumn(1, _T("2012.02����"), LVCFMT_CENTER, rect.Width()/3, 1);   
	m_programLangList.InsertColumn(2, _T("2011.02����"), LVCFMT_CENTER, rect.Width()/3, 2); 
	// Ϊ�б���ͼ�ؼ����ȫ��ѡ�к�դ����   
	m_programLangList.SetExtendedStyle(m_programLangList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES|LVS_EX_ONECLICKACTIVATE);   

	m_programLangList.SetItemCount(4);

	// ���б���ͼ�ؼ��в����б���������б������ı�   
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

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CtestlistDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CtestlistDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CtestlistDlg::OnNMClickProgramLangList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	

	NMLISTVIEW *pNMListView = (NMLISTVIEW*)pNMHDR;   

	if (-1 != pNMListView->iItem)        // ���iItem����-1����˵�����б��ѡ��   
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
	  m_Row = pNMListView->iItem;//���ѡ�е���
	 m_Col = pNMListView->iSubItem;//���ѡ����
	//CEdit m_edit;

	if   (pNMListView->iSubItem != 0) //���ѡ���������;
	{
		m_programLangList.GetSubItemRect(m_Row,m_Col,LVIR_LABEL,rc);//��������RECT��
		m_edit.SetParent(&m_programLangList);//ת������Ϊ�б���е�����
		m_edit.MoveWindow(rc);//�ƶ�Edit��RECT���ڵ�λ��;
		m_edit.SetWindowText(m_programLangList.GetItemText(m_Row,m_Col));//���������е�ֵ����Edit�ؼ��У�
		m_edit.ShowWindow(SW_SHOW);//��ʾEdit�ؼ���
		m_edit.SetFocus();//����Edit����
		m_edit.ShowCaret();//��ʾ���
		m_edit.SetSel(-1);//������ƶ������


	}

	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}


void CtestlistDlg::OnEnKillfocusEdit1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str;
	if(m_edit.m_hWnd!=NULL)
	{
		m_edit.GetWindowText(str);//ȡ�ñ༭�������
		m_programLangList.SetItemText(m_Row,m_Col,str);//�������ݸ��µ�CListCtrl��
		m_edit.ShowWindow(SW_HIDE);//���ر༭��
	}
}
