// Copyright 2021 common-server Authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

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