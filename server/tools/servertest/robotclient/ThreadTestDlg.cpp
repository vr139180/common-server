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

#include <windows.h>

#include "ThreadTestDlg.h"
#include "winsock2.h"
#include "ScriptDlg.h"
#include <cmsLib/Log.h>
#include <cmsLib/util/ShareUtil.h>

CString			g_ip;
int				g_port;
std::string		g_xml;

CWindow			*g_mainwnd =0;
LogSaveUtil		*g_logsave = 0;

CThreadTestDlg::CThreadTestDlg():
	m_hSocket( INVALID_SOCKET)
{
	m_user =0;
	tests_ =0;
	thread_num_ =0;
	m_usernum =1;
	m_startplayer =_T("");
	m_playernums =_T("");
	m_robotstatus =_T("");

	m_urladdr = _T("");
	
	need_recon = false;
	g_mainwnd =this;
	g_logsave = &logsave_util_;
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

	char text[218] = { 0 };
	GetPrivateProfileString("run", "urladdr", "", text, sizeof(text), filepath.c_str());
	m_urladdr = text;

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

	upload_util_.stop_uploads();

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

LRESULT CThreadTestDlg::OnBnClearLogs(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	upload_util_.clear_logs();

	return 0;
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
				m_urladdr = ack->urladdr().c_str();
				m_openprefix = ack->openprefix().c_str();

				m_startplayer.Format("%d", ack->startuserid());
				m_playernums.Format("%d", ack->users());
				
				m_user = ack->startuserid();
				m_usernum = ack->users();

				logsave_util_.dbuser_ = ack->dbuser().c_str();
				logsave_util_.dbpwd_ = ack->dbpwd().c_str();
				logsave_util_.dbname_ = ack->dbname().c_str();
				logsave_util_.dbip_ = ack->dbip().c_str();
				
				m_robotstatus = "config success";
				DoDataExchange(DDX_LOAD);

				logInfo(out_runtime, "robot client regist to robot server....");
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

			DoDataExchange(DDX_LOAD);
			StartAutoRobot();

			m_robotstatus = "robot starting";
			DoDataExchange(DDX_LOAD);
		}
		break;

		case ROBOTOTEST_ROBOT_STOP_ACK:
		{
			Robot_Stop_Ack* ack = reinterpret_cast<Robot_Stop_Ack*>(ret);
			m_robotstatus = "stop login";

			DoDataExchange(DDX_LOAD);
			StopAutoRobot();

			//shutdown log,and reinit
			logShutdown;

			std::string logfile = "log_settings.ini";
			logInit(logfile.c_str());
		}
		break;

		case ROBOTOTEST_UPLOADLOGSROBOT_NTF:
		{
			m_robotstatus = "start upload logs...";

			DoDataExchange(DDX_LOAD);
			StopAutoRobot();

			//shutdown log,and reinit
			logShutdown;

			std::string logfile = "log_settings.ini";
			logInit(logfile.c_str());

			upload_util_.start_uploads();
		}
		break;
		case ROBOTOTEST_CLEARLOGSROBOT_NTF:
		{
			m_robotstatus = "delete all log fiels...";
			DoDataExchange(DDX_LOAD);

			upload_util_.clear_logs();
		}
		break;
		case ROBOTOTEST_USERPREFIXROBOT_NTF:
		{
			Robot_UserPrefixChg_ntf* ack = reinterpret_cast<Robot_UserPrefixChg_ntf*>(ret);
			m_openprefix = ack->prefix().c_str();

			m_robotstatus.Format("openid prefix to %s", (LPCTSTR)m_openprefix);
			DoDataExchange(DDX_LOAD);

			for (int i = 0; i < thread_num_; ++i)
			{
				tests_[i].set_openprefix((LPCTSTR)m_openprefix);
			}
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

void CThreadTestDlg::send_proto(BasicProtocol* msg)
{
	uint8_t *pbuf = 0;
	uint32_t len = 0;

	uint32_t len2 = 0;
	while ((len2 = ::send(m_hSocket, (const char*)pbuf, len, 0)) != SOCKET_ERROR)
	{
		len -= len2;
		if (len <= 0)
			break;
		pbuf = pbuf + len2;
	}
}

void CThreadTestDlg::robot_login()
{
	Robot_Config_Req req;
	req.set_version(ROBOT_VERSION);
	req.set_robotname(dlg.m_logon.GetBuffer(0));
	req.set_robotpwd("123456");

	send_proto( &req);
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

	WritePrivateProfileString("run", "urladdr", m_urladdr, filename);

	ScriptDlg dlg;
	dlg.seturladdr((LPCTSTR)m_urladdr);

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

		tests_[i].set_thread(m_user + (i * MAXCASE_PRETHREAD), nums, (LPCTSTR)m_urladdr, (LPCTSTR)m_openprefix);

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

LRESULT CThreadTestDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	if( tests_)
		delete[] tests_;
	tests_ =0;

	CloseDialog( 0);
	return 0;
}

LRESULT CThreadTestDlg::OnBeginUploadLog(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	m_robotstatus = "-----begin upload log-----";
	DoDataExchange(DDX_LOAD);

	PRO::Robot_StartRecord_req req;
	req.set_robotid(m_robotid);

	this->send_proto(&req);

	SetTimer(20000, 1000);

	return 0;
}


LRESULT CThreadTestDlg::OnUploadLog(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	if (wParam == 0)
		return 0;

	return 0;
}

LRESULT CThreadTestDlg::OnEndUploadLog(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	m_robotstatus = ShareUtil::str_format("-----end upload log. totle:%d-----", logsave_util_.get_lognum()).c_str();
	DoDataExchange(DDX_LOAD);

	Robot_EndRecord_req req;
	req.set_robotid(m_robotid);
	req.set_logs(logsave_util_.get_lognum());

	this->send_proto(&req);

	KillTimer(20000);
	upload_util_.stop_uploads();
	logsave_util_.stop_save();

	return 0;
}

LRESULT CThreadTestDlg::OnTimer(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	if (1000 == wParam)
	{
		if (0 == tests_)
			KillTimer(wParam);
	}
	else if (wParam == 10000)
	{
		this->OnReceive();
	}
	else if (wParam == 20000)
	{
		m_robotstatus = ShareUtil::str_format("upload log:%d...", logsave_util_.get_lognum()).c_str();
		DoDataExchange(DDX_LOAD);

		Robot_Record_req req;
		req.set_robotid(m_robotid);
		req.set_logs(logsave_util_.get_lognum());

		this->send_proto(&req);
	}

	return TRUE;
}
