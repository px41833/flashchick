#ifndef TEST1_H
#define TEST1_H

#pragma once
#include "stdafx.h"

//DWORD m_dwThread;

typedef struct rect
{
	CString yb_code; //����
	CString yb_name;//����
	CString yb_sale;//��������1��2��
	CString yb_price;//�趨�۸�
	CString yb_harden;//��ͣ��
	CString yb_trop;//��ͣ��
	CString yb_number;//��������
}YB_PARAM;

typedef struct committime
{
	time_t time_localcurrent;
	int settimediff;
	int com_check;
}COM_TIME;

#define TALK_MESSAGE WM_USER+0x01
#define MESSAGE_LOGON_SUCCESS  WM_USER+0x02
#define MESSAGE_SYNC_TIME      WM_USER+0x03
#define MESSAGE_FIRM_INFO      WM_USER+0x04
#define MESSAGE_ADD_YB_LIST      WM_USER+0x05
#define MESSAGE_SET_COMMIT_TIME      WM_USER+0x06  //�û����õĶ����ύʱ��
#define MESSAGE_SET_COMMIT_TIME_DIFF      WM_USER+0x07  //�û����õĶ����ύ���ʱ��
#define MESSAGE_RE_LOGON      WM_USER+0x08
#define MESSAGE_WEEK_ORDER      WM_USER+0x09

#endif