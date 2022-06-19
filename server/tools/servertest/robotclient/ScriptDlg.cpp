// ScriptDlg.cpp : implementation file
//

#include "stdafx.h"

#include "ScriptDlg.h"

#include <string>

ScriptDlg::ScriptDlg():
	run_script_(_T("")),
	fun_desc_(_T(""))
{
}

ScriptDlg::~ScriptDlg()
{
}

LRESULT ScriptDlg::OnBnClickedRunBut(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	DoDataExchange( DDX_SAVE);

	if( run_script_.GetLength() > 0)
	{
		std::string cmd ="cmd:";
		cmd +=(LPCTSTR)run_script_;

		int ret =scrimpl_.RunLuaShellCmd( cmd);
		if( ret != 1)
			MessageBox("½Å±¾error!");
		else
		{
			run_result_ =scrimpl_.ret_desc_ + "\r\n" + run_result_;
			status_ =scrimpl_.get_status().c_str();

			DoDataExchange( DDX_LOAD);
		}
	}
	else
	{
		MessageBox("ÇëÊäÈë½Å±¾!");
	}	

	return TRUE;
}

LRESULT ScriptDlg::OnWrite(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    if( wParam !=NULL)
        run_result_ =*((CString*)wParam) + "\r\n" +run_result_;

    //UpdateData( false);
	m_resedit.SetWindowText( run_result_);

    return 0;
}

LRESULT ScriptDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CenterWindow();

    this->scrimpl_.set_wnd( this->m_hWnd);

	DoDataExchange( DDX_LOAD);

	regist();

	return TRUE;
}

void ScriptDlg::add_function( const char* fun, const char* desc)
{
	int ind =support_fun_.AddString( fun);
	funs_[ind] =(desc == 0?"":desc);
}

std::string ScriptDlg::get_function_desc( int ind)
{
	std::string ret ="";

	std::map<int,std::string>::iterator fiter =funs_.find( ind);
	if( fiter != funs_.end())
		ret =fiter->second;

	return ret;
}

LRESULT ScriptDlg::OnLbnDblclkSupportcombo(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	int sel =support_fun_.GetCurSel();
	if( sel == LB_ERR)
		return TRUE;

	support_fun_.GetText( sel, run_script_);
	std::string desc =get_function_desc( sel);
	fun_desc_ =desc.c_str();

	DoDataExchange( DDX_LOAD);

	return TRUE;
}

LRESULT ScriptDlg::OnBnClickedClearbtn(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	DoDataExchange( DDX_SAVE);

	run_result_ ="";

	DoDataExchange( DDX_LOAD);

	return TRUE;
}
