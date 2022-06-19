#include "stdafx.h"

#include <windows.h>

#include "ThreadTestDlg.h"
#include "winsock2.h"
#include "ScriptDlg.h"

CWindow			*g_mainwnd =0;

CThreadTestDlg::CThreadTestDlg()
{
	m_port =0;
	m_ip =_T("");
	m_ut = _T("");
	m_uid = 0;
	
	g_mainwnd =this;
}

LRESULT CThreadTestDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
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

	UIAddChildWindowContainer( m_hWnd);

	char filename[1024];
	GetCurrentDirectory( 1024, filename);
	std::string filepath = NS_STL::string(filename) + "\\robotclient.ini";

	char text[256];
	GetPrivateProfileString( "run", "lgsip", "127.0.0.1", text, sizeof(text), filepath.c_str());
	m_ip= text;
	m_port = GetPrivateProfileInt( "run", "lgsport", 15000, filepath.c_str());

	GetPrivateProfileString("run", "ut", "D6976CEF6FC145C68CE983B05BA0E460", text, sizeof(text), filepath.c_str());
	m_ut = text;
	m_uid = GetPrivateProfileInt("run", "uid", 0, filepath.c_str());

	DoDataExchange( DDX_LOAD);

	SetTimer( 10000, 1000);

	return TRUE;
}

LRESULT CThreadTestDlg::OnBnClickedScriptbutton(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	DoDataExchange( DDX_SAVE);

	char filename[260];
	GetModuleFileName(NULL, filename, sizeof(filename));
	(_tcsrchr(filename,'\\'))[1] = 0;
	strcat( filename, "robotclient.ini");

	WritePrivateProfileString( "run", "lgsip", m_ip, filename);
	CString port;
	port.Format( "%u", m_port);
	WritePrivateProfileString( "run", "lgsport", port, filename);

	WritePrivateProfileString("run", "ut", m_ut, filename);
	port.Format("%d", m_uid);
	WritePrivateProfileString("run", "uid", port, filename);

	ScriptDlg dlg;
	dlg.setip( (LPCTSTR)m_ip);
	dlg.setport( m_port);
	dlg.setuid(m_uid);
	dlg.setut((LPCTSTR)m_ut);

	dlg.DoModal();

	return TRUE;
}

LRESULT CThreadTestDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CloseDialog( 0);
	return 0;
}
