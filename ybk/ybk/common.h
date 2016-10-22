#ifndef TEST1_H
#define TEST1_H

#pragma once
#include "stdafx.h"

//DWORD m_dwThread;

typedef struct rect
{
	CString yb_code; //代码
	CString yb_name;//名称
	CString yb_sale;//买卖方向1买2卖
	CString yb_price;//设定价格
	CString yb_harden;//涨停价
	CString yb_trop;//跌停价
	CString yb_number;//够买数量
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
#define MESSAGE_SET_COMMIT_TIME      WM_USER+0x06  //用户设置的定单提交时间
#define MESSAGE_SET_COMMIT_TIME_DIFF      WM_USER+0x07  //用户设置的定单提交误差时间
#define MESSAGE_RE_LOGON      WM_USER+0x08
#define MESSAGE_WEEK_ORDER      WM_USER+0x09

#endif