
// ybk2Dlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


// Cybk2Dlg �Ի���
class Cybk2Dlg : public CDialogEx
{
// ����
public:
	Cybk2Dlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_YBK2_DIALOG };

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
	afx_msg void OnCbnSelchangeCombo1();
	
	afx_msg void OnBnClickedButtonLogin();

	HANDLE hThread;
		fd_set fdrecv;
	//void  ThreadFunc();
};
