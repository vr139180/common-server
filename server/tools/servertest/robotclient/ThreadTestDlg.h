#pragma once

#include "stdafx.h"

#include <list>

#include "Resource.h"
#include "ThreadWrapper.h"
#include "MsgNotify.h"
#include "LogonDlg.h"
#include <gameLib/protobuf/Proto_all.h>

using namespace PRO;

#define WM_INFONOTIFY  WM_USER+0x100
#define WM_ERRORNOTIFY  WM_USER+0x101

#define MAXCASE_PRETHREAD	50

#define MAX_BUFFER_LEN	1024*16

#define ROBOT_SOCKET_BUF		1024*512	//512k

class CAboutDlg :public CDialogImpl<CAboutDlg>
{
public:
	enum { IDD = IDD_ABOUTBOX };

	BEGIN_MSG_MAP(CAboutDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		CenterWindow(GetParent());
		return TRUE;
	}

	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		EndDialog(wID);
		return 0;
	}
};

class CThreadTestDlg : public CAxDialogImpl<CThreadTestDlg>, public CUpdateUI<CThreadTestDlg>,
	public CMessageFilter, public CIdleHandler, public CWinDataExchange<CThreadTestDlg>
{
public:
	enum { IDD = IDD_THREADTEST_DIALOG };

	CThreadTestDlg();

	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		return CWindow::IsDialogMessage(pMsg);
	}

	virtual BOOL OnIdle()
	{
		return FALSE;
	}

	BEGIN_UPDATE_UI_MAP(CThreadTestDlg)
	END_UPDATE_UI_MAP()
	
	BEGIN_MSG_MAP(CThreadTestDlg)
		MESSAGE_HANDLER( WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
		MESSAGE_HANDLER(WM_CLOSE, OnDestroy)

		MESSAGE_HANDLER(WM_INFONOTIFY, OnShowNotify)
		MESSAGE_HANDLER(WM_ERRORNOTIFY, OnShowError)

		COMMAND_ID_HANDLER(IDOK, OnBnClickedOk)
		COMMAND_ID_HANDLER(IDCANCEL, OnBnClickedCancel)

		COMMAND_ID_HANDLER(IDC_SCRIPTBUTTON, OnBnClickedScriptbutton)
	END_MSG_MAP()

	BEGIN_DDX_MAP( CThreadTestDlg)
		DDX_TEXT( IDC_EDIT2, m_ip)
		DDX_INT( IDC_EDIT4, m_port)
		DDX_CONTROL_HANDLE( IDOK, m_startbut)
		DDX_CONTROL_HANDLE( IDCANCEL, m_endbut)
		DDX_TEXT( IDC_STARTNUM, m_startplayer)
		DDX_TEXT( IDC_CNTNUM, m_playernums)
		DDX_TEXT( IDC_GWIP, m_robotsvrip)
		DDX_TEXT( IDC_ROBOTSTATUS, m_robotstatus)
	END_DDX_MAP()

public:
	LRESULT OnTimer(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	LRESULT OnShowNotify(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnShowError(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedOk(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	LRESULT OnBnClickedCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedScriptbutton(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	void CloseDialog(int nVal)
	{
		DestroyWindow();
		::PostQuitMessage(nVal);
	}

protected:
	void show_notify( MsgNotify *msg);
	void show_error( MsgNotify *msg);
	bool connect_2_server();
	ULONGLONG GetMillisecondTime();

	virtual void OnReceive();
	virtual void OnClose();

	void robot_login();

protected:
	char			buffer_[ROBOT_SOCKET_BUF];
	int				read_buffer_pos_;

	ThreadWrapper	*tests_;
	int				thread_num_;

public:
	CLogonDlg		dlg;
	CButton			m_startbut;
	CButton			m_endbut;

	CListViewCtrl	m_list;
	int				m_usernum;
	int				m_user;
	int				m_userrange;

	SOCKET			m_hSocket;

	bool			need_recon;
	ULONGLONG		last_time;
	ULONGLONG		current_time;

protected:
	void ReportError( char* fmt, ...);
	void StartAutoRobot();
	void StopAutoRobot();

public:
	CString			m_ip;
	int				m_port;

	CString			m_lgsip;
	int				m_lgsport;

	CString			m_startplayer;
	CString			m_playernums;
	CString			m_playerrange;
	CString			m_robotstatus;
	CString			m_robotsvrip;

	int				m_robotid;

	CString			log_;
};
