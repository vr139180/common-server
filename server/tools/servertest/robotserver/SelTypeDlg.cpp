#include "stdafx.h"
#include "SelTypeDlg.h"
#include "OptionUtil.h"

extern OptionUtil g_opt;

CSelTypeDlg::CSelTypeDlg()
{
}

LRESULT CSelTypeDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CenterWindow();

	lst_bev_ = GetDlgItem( IDC_LST_BEV);
	lst_bev_.SetExtendedListViewStyle(LVS_EX_FULLROWSELECT);

	lst_bev_.InsertColumn(0, _T("testcase name"), LVCFMT_RIGHT, 200);

	NS_STL::list<TestCaseXml>::iterator iter =g_opt.testcase_.begin(), eiter =g_opt.testcase_.end();
	for( ; iter != eiter; ++iter)
	{
		TestCaseXml& tc =(*iter);

		S_INT_32 item = lst_bev_.InsertItem(0, tc.name_.c_str());
	}

	return TRUE;  // return TRUE unless you set the focus to a control
}

LRESULT CSelTypeDlg::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	EndDialog(0);

	return TRUE;
}

LRESULT CSelTypeDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	S_INT_32 count = lst_bev_.GetItemCount();
	for ( S_INT_32 i = 0; i < count; i ++ )
	{
		if ( LVIS_SELECTED == lst_bev_.GetItemState( i, LVIS_SELECTED)){
			char text[256] ={0,};
			lst_bev_.GetItemText( i, 0, text, sizeof(text));
			sel_index_ =text;
		}
	}
	
	EndDialog(0);

	return TRUE;
}
