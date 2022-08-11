#include "stdafx.h"

#include <windows.h>

#include "ThreadTestDlg.h"
#include "winsock2.h"
#include "ScriptDlg.h"

#define NET_HEAD_MASK_ZERO_H	0x0000FFFF
#define NET_HEAD_MASK_ZERO_L	0xFFFF0000

#define MAKE_NETHEAD( VAL, PROID, LENS) { VAL = LENS; VAL = (VAL << 16)&NET_HEAD_MASK_ZERO_L; VAL = VAL|(PROID&NET_HEAD_MASK_ZERO_H);}
#define GET_NETHEAD( VAL, PROID, LENS) { LENS = (S_UINT_16)((VAL&NET_HEAD_MASK_ZERO_L)>>16); PROID = (S_UINT_16)(VAL&NET_HEAD_MASK_ZERO_H);}


CString			g_ip;
int				g_port;
std::string		g_xml;

CWindow			*g_mainwnd =0;

CThreadTestDlg::CThreadTestDlg():
	m_hSocket( INVALID_SOCKET)
{
	m_user =0;
	tests_ =0;
	thread_num_ =0;
	m_port =0;
	m_usernum =1;
	m_userrange = 1;
	m_ip =_T("");
	m_startplayer =_T("");
	m_playernums =_T("");
	m_playerrange =_T("");
	m_robotsvrip =_T("");
	m_robotstatus =_T("");
	
	need_recon = false;
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

	m_user =0;

	m_list = GetDlgItem( IDC_LIST1);
	m_list.SetExtendedListViewStyle(LVS_EX_FULLROWSELECT);

	m_list.InsertColumn(0, _T("user"), LVCFMT_LEFT, 100);
	m_list.InsertColumn(1, _T("msg"), LVCFMT_LEFT, 500);

	DoDataExchange( DDX_LOAD);

	SetTimer( 10000, 1000);

	return TRUE;
}

LRESULT CThreadTestDlg::OnBnClickedOk(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_list.DeleteAllItems();

	char filename[1024];
	GetCurrentDirectory( 1024, filename);
	std::string filepath = NS_STL::string(filename) + "\\robotclient.ini";

	char text[256];
	GetPrivateProfileString( "run", "robotserverip", "127.0.0.1", text, sizeof(text), filepath.c_str());
	dlg.m_ip = text;
	dlg.m_port = GetPrivateProfileInt( "run", "robotserverport", 12345, filename);
	GetPrivateProfileString( "run", "logonuser", "robot001", text, sizeof(text), filepath.c_str());
	dlg.m_logon = text;

	if( dlg.DoModal() != IDOK)
		return TRUE;

	WritePrivateProfileString( "run", "robotserverip", dlg.m_ip, filepath.c_str());
	CString robotserverport;
	robotserverport.Format( "%u", dlg.m_port);
	WritePrivateProfileString( "run", "robotserverport", robotserverport, filepath.c_str());
	WritePrivateProfileString( "run", "logonuser", dlg.m_logon, filepath.c_str());
	
	if(!connect_2_server())
		return FALSE;
	last_time = GetMillisecondTime();
	return TRUE;
}

void CThreadTestDlg::OnReceive()
{
	if( need_recon){
		current_time = GetMillisecondTime();
		if(current_time - last_time > 5000){
			if(!connect_2_server())
				last_time = GetMillisecondTime();
		}else{
			return;
		}
	}
	if( m_hSocket == INVALID_SOCKET){
		return;
	}

	char *pbuf = buffer_ + read_buffer_pos_;
	int len = MAX_PACK_LEN - read_buffer_pos_;

	int rlen = ::recv(m_hSocket, pbuf, len, 0);

	if (rlen == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			::closesocket(m_hSocket);
			m_hSocket = INVALID_SOCKET;
			//reset recvbuf
			read_buffer_pos_ = 0;

			this->OnClose();

			return;
		}

		if (read_buffer_pos_ < sizeof(S_UINT_32))
			return;
	}

	if (rlen >= 0)
		read_buffer_pos_ += rlen;

	//分析现有数据
	if (read_buffer_pos_ < sizeof(S_UINT_32))
		return;
	pbuf = buffer_;

	S_UINT_32 hd = *((S_UINT_32*)pbuf);

	S_UINT_16 len2 = 0;
	S_UINT_16 proid = 0;
	GET_NETHEAD(hd, proid, len2);

	if (len2 > read_buffer_pos_ - sizeof(S_UINT_32))
		return;

	pbuf += sizeof(S_UINT_32);

	BasicProtocol *ret = ProtocolFactory::instance()->iid_to_proto(proid, pbuf, len2);

	//移动内存
	read_buffer_pos_ -= sizeof(S_UINT_32);
	read_buffer_pos_ -= len2;
	pbuf += len2;
	memmove(&(buffer_[0]), pbuf, read_buffer_pos_);

	if (ret)
	{
		switch (proid)
		{
		case ROBOTOTEST_ROBOT_CONFIG_ACK:
		{
			Robot_Config_Ack* ack = dynamic_cast<Robot_Config_Ack*>(ret);
			if (0 == ack->result())
			{
				m_robotid = ack->robotid();
				m_lgsip = ack->lgsip().c_str();
				m_lgsport = ack->lgsport();
				m_startplayer.Format("%d", ack->startuserid());
				m_playernums.Format("%d", ack->users());
				m_playerrange.Format("%d", ack->usersrange());
				m_user = ack->startuserid();
				m_usernum = ack->users();
				m_userrange = ack->usersrange();
				m_robotstatus = "config success";
				m_robotsvrip = m_lgsip;
				DoDataExchange(DDX_LOAD);
			}
			else
			{
				ReportError("get config failed. ret = %d", ack->result());
				m_startbut.EnableWindow(TRUE);
			}
		}
		break;

		case ROBOTOTEST_ROBOT_START_ACK:
		{
			Robot_Start_Ack* ack = reinterpret_cast<Robot_Start_Ack*>(ret);
			
			g_xml = ack->behavior(0).c_str();
			m_robotstatus = "begin to login";

			DoDataExchange(DDX_LOAD);
			StartAutoRobot();

			m_robotstatus = "Threads running";
			DoDataExchange(DDX_LOAD);
		}
		break;

		case ROBOTOTEST_ROBOT_STOP_ACK:
		{
			Robot_Stop_Ack* ack = reinterpret_cast<Robot_Stop_Ack*>(ret);
			m_robotstatus = "stop login";

			DoDataExchange(DDX_LOAD);
			StopAutoRobot();
		}
		break;
		}
	}
}

void CThreadTestDlg::OnClose()
{
	BOOL bHandle =TRUE;
	OnBnClickedCancel( 0, 0, 0, bHandle);

	m_robotstatus ="stop test....";
	DoDataExchange( DDX_LOAD);

	if( m_hSocket != INVALID_SOCKET)
	{
		::closesocket( m_hSocket);
		m_hSocket =INVALID_SOCKET;
	}
}

LRESULT CThreadTestDlg::OnBnClickedCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	for( int s =0; s < thread_num_; ++s)
		tests_[s].stop_thread();

	if( tests_)
		delete[] tests_;
	tests_ =0;
	thread_num_ =0;

	m_startbut.EnableWindow( TRUE);

	return TRUE;
}

LRESULT CThreadTestDlg::OnShowNotify(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	show_notify( (MsgNotify*)wParam);

	return TRUE;
}

LRESULT CThreadTestDlg::OnShowError(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	show_error( (MsgNotify*)wParam);

	return TRUE;
}

void CThreadTestDlg::show_notify( MsgNotify *msg)
{
	int ss =m_list.GetItemCount();
	for( int i =0; i < ss; ++i)
	{
		int iid =(int)m_list.GetItemData( i);
		if( iid == msg->userid_)
		{
			m_list.SetItemText( i, 1, msg->msg_);
			delete msg;
			return;
		}
	}

	//sort
	for( int i =0; i < ss; ++i)
	{
		int iid =(int)m_list.GetItemData( i);
		if( iid > msg->userid_)
		{
			ss =i -1 ;
			if( ss < 0)
				ss =0;

			break;
		}
	}

	CString str;
	str.Format( "%d", msg->userid_);
	long lid =m_list.InsertItem( ss, str);

	m_list.SetItemText( lid, 1, msg->msg_);
	m_list.SetItemData( lid, msg->userid_);

	delete msg;
}

void CThreadTestDlg::show_error( MsgNotify *msg)
{
	ReportError( "user:%d %s", msg->userid_, msg->msg_);
	delete msg;
}

bool CThreadTestDlg::connect_2_server(){
	if( this->m_hSocket != INVALID_SOCKET)
	{
		::closesocket( m_hSocket);
		m_hSocket =INVALID_SOCKET;
	}

	m_hSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	int one =1;
	::setsockopt( m_hSocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&one, sizeof( one));

	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family		= AF_INET;
	serv_addr.sin_addr.s_addr	= inet_addr( (LPCTSTR)dlg.m_ip);
	serv_addr.sin_port			= htons( dlg.m_port);

	if( ::connect( m_hSocket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR)
	{
		int err =WSAGetLastError();
		::closesocket( m_hSocket);
		m_hSocket =INVALID_SOCKET;

		//MessageBox("connect to robot server failed!");
		return FALSE;
	}
	else
	{
		u_long ll =1;
		ioctlsocket( m_hSocket, FIONBIO, &ll);
	}

	read_buffer_pos_ =0;

	m_startbut.EnableWindow( FALSE);

	robot_login();

	need_recon = false;

	return TRUE;
}

void CThreadTestDlg::robot_login()
{
	Robot_Config_Req req;
	req.set_version(ROBOT_VERSION);
	req.set_robotname(dlg.m_logon.GetBuffer(0));
	req.set_robotpwd("123456");

	char buf[SINGLE_PACK_LEN] = { 0 };
	char *pbuf = buf + sizeof(S_UINT_32);

	S_INT_32 len = (S_INT_32)req.ByteSizeLong();
	S_UINT_16 proiid = ProtocolFactory::instance()->proto_to_iid(&req);

	req.SerializeToArray(pbuf, len);

	S_UINT_32 pro_head = 0;
	MAKE_NETHEAD(pro_head, proiid, len);

	*((S_UINT_32*)buf) = pro_head;

	int totlen = len + sizeof(S_UINT_32);
	int len2 = 0;
	pbuf = buf;

	::send(m_hSocket, pbuf, totlen, 0);
}

ULONGLONG CThreadTestDlg::GetMillisecondTime()
{
	FILETIME stime={0};
	GetSystemTimeAsFileTime( &stime);
	ULONGLONG ll =*((PULONGLONG)&stime);
	return ll/10000;
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

	ScriptDlg dlg;
	dlg.setip( (LPCTSTR)m_ip);
	dlg.setport( m_port);

	dlg.DoModal();

	return TRUE;
}

void CThreadTestDlg::ReportError( char* pFmt, ...)
{
	char text[32];
	CTime tm = CTime::GetCurrentTime();
	sprintf(text, "%.2d %.2d:%.2d:%.2d ", tm.GetDay(), tm.GetHour(), tm.GetMinute(), tm.GetSecond());

	va_list args;
	char buf[1024];
	va_start(args, pFmt);
	vsprintf(buf, pFmt, args);
	log_ = CString(text) + CString(buf) + "\r\n" + log_;
	GetDlgItem(IDC_EDT_LOG).SetWindowText( log_);
}

void CThreadTestDlg::StartAutoRobot()
{
	g_ip = m_lgsip;
	g_port = m_lgsport;

	if( tests_)
		delete[] tests_;
	thread_num_ = m_usernum / MAXCASE_PRETHREAD;
	if( m_usernum - thread_num_*MAXCASE_PRETHREAD > 0)
		++thread_num_;

	int pos =m_usernum;
	tests_ =new ThreadWrapper[thread_num_];
	for( int i=0 ; i < thread_num_ ; ++i)
	{
		int nums =MAXCASE_PRETHREAD;
		if( i + 1 == thread_num_)
		{
			if( pos - MAXCASE_PRETHREAD < 0)
				nums =pos;
		}

		tests_[i].set_thread( m_user + ( i * MAXCASE_PRETHREAD * m_userrange), nums, m_userrange);

		tests_[i].start_thread();

		pos -= MAXCASE_PRETHREAD;
		if( pos <= 0)
			break;
	}

	SetTimer( 1000, 1000, NULL);
}

void CThreadTestDlg::StopAutoRobot()
{
	for( int s =0; s < thread_num_; ++s)
		tests_[s].set_cancel();

	for( int s =0; s < thread_num_; ++s)
		tests_[s].stop_thread();

	if( tests_)
		delete[] tests_;
	tests_ =0;
	thread_num_ =0;
}

LRESULT CThreadTestDlg::OnTimer(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	if ( 1000 == wParam)
	{
		if( 0 == tests_)
			KillTimer( wParam);
	}
	else if( wParam == 10000)
	{
		this->OnReceive();
	}

	return TRUE;
}

LRESULT CThreadTestDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	if( tests_)
		delete[] tests_;
	tests_ =0;

	CloseDialog( 0);
	return 0;
}
