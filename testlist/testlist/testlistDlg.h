
// testlistDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CtestlistDlg �Ի���
class CtestlistDlg : public CDialog
{
// ����
public:
	CtestlistDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TESTLIST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_programLangList;
	afx_msg void OnNMClickProgramLangList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkProgramLangList(NMHDR *pNMHDR, LRESULT *pResult);
	CEdit m_edit;
	afx_msg void OnEnKillfocusEdit1();
	int m_Row;
	int m_Col;
};
