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

#pragma once

#include "stdafx.h"

#include "CommandTestImpl.h"
#include "resource.h"
#include <string>
#include <map>

class ScriptDlg : public CAxDialogImpl<ScriptDlg>, public CUpdateUI<ScriptDlg>,
	public CWinDataExchange<ScriptDlg>
{
public:
	ScriptDlg();
	virtual ~ScriptDlg();

	enum { IDD = IDD_SCRIPTDLG };

	BEGIN_UPDATE_UI_MAP(ScriptDlg)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(ScriptDlg)
		MESSAGE_HANDLER( WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER( WM_USER+0x200, OnWrite)
		MESSAGE_HANDLER( WM_CLOSE, OnClose)

		COMMAND_HANDLER(IDC_RUNBUTTON, BN_CLICKED, OnBnClickedRunBut)
		COMMAND_HANDLER(IDC_SUPPORTCOMBO, LBN_DBLCLK, OnLbnDblclkSupportcombo)
		COMMAND_HANDLER(IDC_CLEARBTN, BN_CLICKED, OnBnClickedClearbtn)
	END_MSG_MAP()

	BEGIN_DDX_MAP( ScriptDlg)
		DDX_TEXT( IDC_RUNSCEDIT, run_script_);
		DDX_TEXT( IDC_DESC, fun_desc_);
		DDX_TEXT( IDC_STATUSEDIT, status_);
		DDX_TEXT( IDC_RESULTEDIT, run_result_);

		DDX_CONTROL_HANDLE( IDC_SUPPORTCOMBO, support_fun_);
		DDX_CONTROL_HANDLE( IDC_RESULTEDIT, m_resedit);
	END_DDX_MAP()

public:
	CListBox			support_fun_;
	CEdit				m_resedit;
	CommandTestImpl		scrimpl_;

	CString		fun_desc_;
	CString		run_script_;
	CString		status_;
	CString		run_result_;

	std::map<int,std::string>	funs_;

	void add_function( const char* fun, const char* desc);
	std::string get_function_desc( int ind);

public:
	void seturladdr(std::string addr) {
		scrimpl_.url_addr_ = addr;
	}

	void setuid(S_INT_64 uid) {
		scrimpl_.user_iid_ = uid;
	}
	void setut(S_INT_64 ut) {
		scrimpl_.user_token_ = ut;
	}

	void regist();

public:
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnWrite(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/){
		this->EndDialog( 0);
		return TRUE;
	}

	LRESULT OnLbnDblclkSupportcombo(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedClearbtn(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedRunBut(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};
