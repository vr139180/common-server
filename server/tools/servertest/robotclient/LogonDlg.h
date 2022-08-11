#pragma once

#include "stdafx.h"

#include "resource.h"

class CLogonDlg : public CDialogImpl<CLogonDlg>, public CWinDataExchange<CLogonDlg>
{
public:
	enum { IDD = IDD_LOGONDLG };

	CLogonDlg();

	BEGIN_DDX_MAP( CLogonDlg)
		DDX_TEXT( IDC_ROBOTIP, m_ip)
		DDX_INT( IDC_ROBOTPORT, m_port)
		DDX_TEXT( IDC_LOGONNAME, m_logon)
	END_DDX_MAP()

	BEGIN_MSG_MAP(CSelTypeDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDCANCEL, OnClose)
		COMMAND_ID_HANDLER(IDOK, OnBnClickedOk)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedOk(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnClose(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		return CWindow::IsDialogMessage(pMsg);
	}

	virtual BOOL OnIdle()
	{
		return FALSE;
	}

public:
	CString		m_ip;
	int			m_port;
	CString		m_logon;
};
