
// ybkDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CybkDlg 对话框
class CybkDlg : public CDialogEx
{
// 构造
public:
	CybkDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_YBK_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonLogin();

public:
	DWORD MainThreadID;
	CListCtrl m_BuySaleList;
	CListCtrl m_ProductList;
	afx_msg void OnNMDblclkListBuySell(NMHDR *pNMHDR, LRESULT *pResult);
	CEdit m_edit;
	int m_Row;
	int m_Col;
	afx_msg void OnEnKillfocusEditList();
	afx_msg void OnBnClickedButtonStartCommit();
	time_t time_localcurrent;
	int settimediff;
	time_t autosynctimediff;
};
