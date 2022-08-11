// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "TcpAcceptListener.h"
#include "OptionUtil.h"
#include "AboutDlg.h"
#include <atlframe.h>
#include <atlctrls.h>
#include <atltime.h>

#include <atlmisc.h>

#include <list>

extern OptionUtil g_opt;

#define ICON_UNKNOW 0
#define ICON_CONNECTED 1
#define ICON_RUNNING 2
#define ICON_STOP 3

class CMainDlg : public CAxDialogImpl<CMainDlg>, public CUpdateUI<CMainDlg>,
		public CMessageFilter, public CIdleHandler, public ITcpUserNotify
{
public:
	enum { IDD = IDD_MAINDLG };

	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		return CWindow::IsDialogMessage(pMsg);
	}

	virtual BOOL OnIdle()
	{
		return FALSE;
	}

	BEGIN_UPDATE_UI_MAP(CMainDlg)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		COMMAND_HANDLER(IDC_BTN_STARTALL, BN_CLICKED, OnBnClickedBtnStartall)
		COMMAND_HANDLER(IDC_BTN_STOPALL, BN_CLICKED, OnBnClickedBtnStopall)
		COMMAND_HANDLER(IDC_BTN_START_SEL, BN_CLICKED, OnBnClickedBtnStartSel)
		COMMAND_HANDLER(IDC_BTN_STOP_SEL, BN_CLICKED, OnBnClickedBtnStopSel)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		// center the dialog on the screen
		CenterWindow();

		// set icons
		HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
			IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
		SetIcon(hIcon, TRUE);
		HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
			IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
		SetIcon(hIconSmall, FALSE);

		// register object for message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->AddMessageFilter(this);
		pLoop->AddIdleHandler(this);

		UIAddChildWindowContainer(m_hWnd);

		lstclients_ = GetDlgItem( IDC_LST_CLIENTS);
		lstclients_.SetExtendedListViewStyle(LVS_EX_FULLROWSELECT);

		CImageList img;
		img.Create( 16, 16, ILC_COLOR8, 4, 0);
		hIcon = LoadIcon ( _Module.GetResourceInstance(), MAKEINTRESOURCE(IDI_ICON_UNKNOW));
		img.AddIcon( hIcon);
		hIcon = LoadIcon ( _Module.GetResourceInstance(), MAKEINTRESOURCE(IDI_ICON_CONNECTED));
		img.AddIcon( hIcon);
		hIcon = LoadIcon ( _Module.GetResourceInstance(), MAKEINTRESOURCE(IDI_ICON_RUNNING));
		img.AddIcon( hIcon);
		hIcon = LoadIcon ( _Module.GetResourceInstance(), MAKEINTRESOURCE(IDI_ICON_STOP));
		img.AddIcon( hIcon);

		lstclients_.SetImageList( img, LVSIL_SMALL);

		lstclients_.InsertColumn(0, _T("会话ID"), LVCFMT_RIGHT, 60);
		lstclients_.InsertColumn(1, _T("名称"), LVCFMT_LEFT, 80);
		lstclients_.InsertColumn(2, _T("IP地址"), LVCFMT_LEFT, 90);
		lstclients_.InsertColumn(3, _T("端口"), LVCFMT_RIGHT, 44);
		lstclients_.InsertColumn(4, _T("时间"), LVCFMT_RIGHT, 60);
		lstclients_.InsertColumn(5, _T("开始ID"), LVCFMT_RIGHT, 50);
		lstclients_.InsertColumn(6, _T("用户数"), LVCFMT_RIGHT, 50);
		lstclients_.InsertColumn(7, _T("用户范围"), LVCFMT_RIGHT, 50);

		if ( !g_opt.load( "robotserver.ini"))
		{
			ATLTRACE("load config file failed.\n");
			DestroyWindow();
			return -1;
		}

		NS_STL::map<int, OptionUtil::RobotDesctiption>::iterator it = g_opt.robots.begin();
		while ( it != g_opt.robots.end())
		{
			OptionUtil::RobotDesctiption& des = it->second;

			char text[256];
			sprintf(text, "%u", 0);
			S_INT_32 item = lstclients_.InsertItem(0, text, ICON_UNKNOW);
			sprintf(text, "%s", des.robotname_.c_str());
			lstclients_.SetItem(item, 1, LVIF_TEXT, text, 0, 0, 0, 0);
			CTime tm = CTime::GetCurrentTime();
			sprintf(text, "%.2d:%.2d:%.2d", tm.GetHour(), tm.GetMinute(), tm.GetSecond());
			lstclients_.SetItem(item, 4, LVIF_TEXT, text, 0, 0, 0, 0);
			sprintf(text, "%u", des.startuserid_);
			lstclients_.SetItem(item, 5, LVIF_TEXT, text, 0, 0, 0, 0);
			sprintf(text, "%u", des.users_);
			lstclients_.SetItem(item, 6, LVIF_TEXT, text, 0, 0, 0, 0);
			sprintf(text, "%u", des.usersrange_);
			lstclients_.SetItem(item, 7, LVIF_TEXT, text, 0, 0, 0, 0);

			lstclients_.SetItemData( item, 0);

			it ++;
		}

		listener_.set_notify( this);

		if ( !listener_.start_listen( g_opt.serverip_.c_str(), g_opt.serverport_))
		{
			ATLTRACE("listen address %s:%u failed.\n", g_opt.serverip_.c_str(), g_opt.serverport_);
			DestroyWindow();
			return -1;
		}

		SetTimer( 1000, 10, NULL);

		return TRUE;
	}

	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CAboutDlg dlg;
		dlg.DoModal();
		return 0;
	}

	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		// TODO: Add validation code 
		CloseDialog(wID);
		return 0;
	}

	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CloseDialog(wID);
		return 0;
	}

	void CloseDialog(int nVal)
	{
		DestroyWindow();
		::PostQuitMessage(nVal);
	}

public:
	virtual void onAccept( SOCKET socket);
	virtual void onClosed( SOCKET socket);
	virtual void onRecvData( char * data, int len, SOCKET socket);
	virtual void onRecvProtocol(S_UINT_16 proid, BasicProtocol* p, SOCKET socket);

private:
	void InsterLog( char* pFmt, ... );
	S_INT_32 FindLstItemBySocket( SOCKET socket);
	S_INT_32 FindLstItemByName( NS_STL::string robotname);

private:
	TcpAcceptListener	listener_;
	CListViewCtrl		lstclients_;
	CString		log_;

public:
	LRESULT OnTimer(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedBtnStartall(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedBtnStopall(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedBtnStartSel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedBtnStopSel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};
