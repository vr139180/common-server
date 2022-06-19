#pragma once

#include "stdafx.h"

#include <list>

#include "Resource.h"


#define WM_INFONOTIFY  WM_USER+0x100
#define WM_ERRORNOTIFY  WM_USER+0x101

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
		MESSAGE_HANDLER(WM_CLOSE, OnDestroy)

		COMMAND_ID_HANDLER(IDC_SCRIPTBUTTON, OnBnClickedScriptbutton)
	END_MSG_MAP()

	BEGIN_DDX_MAP( CThreadTestDlg)
		DDX_TEXT( IDC_EDIT2, m_ip)
		DDX_INT( IDC_EDIT4, m_port)
		DDX_INT(IDC_UID, m_uid)
		DDX_TEXT(IDC_UT, m_ut)
	END_DDX_MAP()

public:
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedScriptbutton(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	void CloseDialog(int nVal)
	{
		DestroyWindow();
		::PostQuitMessage(nVal);
	}

public:
	CString			m_ip;
	int				m_port;

	int				m_uid;
	CString			m_ut;
};
