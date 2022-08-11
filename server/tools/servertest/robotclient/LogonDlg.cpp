#include "stdafx.h"

#include "LogonDlg.h"

CLogonDlg::CLogonDlg():
	 m_ip(_T(""))
	, m_port(0)
	, m_logon(_T("robot001"))
{
}

LRESULT CLogonDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CenterWindow();

	DoDataExchange( DDX_LOAD);

	return TRUE;
}

LRESULT CLogonDlg::OnBnClickedOk(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	DoDataExchange( DDX_SAVE);

	if( m_ip.GetLength() == 0 || m_port == 0 || m_logon.GetLength() == 0)
		return FALSE;

	EndDialog( IDOK);

	return TRUE;
}

LRESULT CLogonDlg::OnClose(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog( 0);

	return TRUE;
}