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
	void setip( std::string ip){
		scrimpl_.lgs_ip_ =ip;
	}
	void setport( int p){
		scrimpl_.lgs_port_ =p;
	}
	void setuid(int uid) {
		scrimpl_.uid = uid;
	}
	void setut(std::string ut) {
		scrimpl_.ut = ut;
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
