#include "StdAfx.h"

#include <gameLib/protobuf/Proto_all.h>
#include <cmsLib/prolib/ProtocolFactory.h>

#include <vector>
#include "maindlg.h"
#include "OptionUtil.h"
#include "SelTypeDlg.h"

extern OptionUtil g_opt;

using namespace PRO;

LRESULT CMainDlg::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	if ( 1000 == wParam)
	{
		listener_.do_main( GetTickCount());
	}

	return 0;
}

LRESULT CMainDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	listener_.stop_listen();

	return 0;
}

void CMainDlg::onAccept( SOCKET socket)
{
	char text[256];
	sockaddr_in sa;
	int   salen;
	salen = sizeof(sa);
	if( getpeername( socket, (sockaddr*)&sa, &salen ) == 0 )
	{
		sprintf( text, "%s:%d", inet_ntoa( sa.sin_addr), ntohs( sa.sin_port));
	}
	else
	{
		sprintf( text, "peer ip error");
	}

	//MessageBox( text);
}

void CMainDlg::onClosed( SOCKET socket)
{
	S_INT_32 item = FindLstItemBySocket( socket);
	if ( -1 != item)
	{
		lstclients_.SetItemData(item, 0);
		lstclients_.SetItem( item, 0, LVIF_TEXT | LVIF_IMAGE, "0", ICON_UNKNOW, 0, 0, 0);
	}
}

void CMainDlg::onRecvData( char * data, int len, SOCKET socket)
{
}

void CMainDlg::onRecvProtocol( S_UINT_16 proid, BasicProtocol* p, SOCKET socket)
{
	USE_PROTOCOL_NAMESPACE;

	switch( proid) 
	{
	case ROBOTOTEST_ROBOT_CONFIG_REQ:
		{
			Robot_Config_Req* req = dynamic_cast <Robot_Config_Req*>(p);

			Robot_Config_Ack* ack = new Robot_Config_Ack();
			ack->set_version(ROBOT_VERSION);

			OptionUtil::RobotDesctiption* des = g_opt.find_robot_byname( req->robotname());
			if ( des)
			{
				if ( des->robotpwd_ != req->robotpwd())
					ack->set_result( -2);
				else
				{
					if ( req->version() != ROBOT_VERSION)
					{
						ack->set_result(-4);
					}
					else
					{
						ack->set_robotid(des->robotid_);
						ack->set_startuserid( des->startuserid_);
						ack->set_users( des->users_);
						ack->set_usersrange( des->usersrange_);
						ack->set_lgsip( des->lgsip_);
						ack->set_lgsport( des->lgsport_);
						ack->set_result(0);

						S_INT_32 item = FindLstItemByName( des->robotname_);
						if ( -1 != item)
						{
							char text[256];
							sprintf(text, "%u", socket);
							lstclients_.SetItem(item, 0, LVIF_TEXT, text, 0, 0, 0, 0);
							sprintf(text, "%s", des->robotname_.c_str());
							lstclients_.SetItem(item, 1, LVIF_TEXT, text, 0, 0, 0, 0);

							sockaddr_in sa;
							int   salen;
							salen = sizeof(sa);
							if( getpeername( socket, (sockaddr*)&sa, &salen ) == 0 )
							{
								sprintf( text, "%s", inet_ntoa( sa.sin_addr));
								lstclients_.SetItem(item, 2, LVIF_TEXT, text, 0, 0, 0, 0);
								sprintf( text, "%d", ntohs( sa.sin_port));
								lstclients_.SetItem(item, 3, LVIF_TEXT, text, 0, 0, 0, 0);
							}
							else
							{
								sprintf( text, "peer ip error");
								lstclients_.SetItem(item, 2, LVIF_TEXT, text, 0, 0, 0, 0);
								sprintf( text, "%d", ntohs( sa.sin_port));
								lstclients_.SetItem(item, 3, LVIF_TEXT, text, 0, 0, 0, 0);
							}

							CTime tm = CTime::GetCurrentTime();
							sprintf(text, "%.2d:%.2d:%.2d", tm.GetHour(), tm.GetMinute(), tm.GetSecond());
							lstclients_.SetItem(item, 4, LVIF_TEXT, text, 0, 0, 0, 0);
							sprintf(text, "%u", des->startuserid_);
							lstclients_.SetItem(item, 5, LVIF_TEXT, text, 0, 0, 0, 0);
							sprintf(text, "%u", des->users_);
							lstclients_.SetItem(item, 6, LVIF_TEXT, text, 0, 0, 0, 0);
							sprintf(text, "%u", des->usersrange_);
							lstclients_.SetItem(item, 7, LVIF_TEXT, text, 0, 0, 0, 0);
							sprintf(text, "%u", 0);
							lstclients_.SetItem(item, 8, LVIF_TEXT, text, 0, 0, 0, 0);
							lstclients_.SetItemData( item, socket);
							lstclients_.SetItem( item, 0, LVIF_IMAGE, "", ICON_CONNECTED, 0, 0, 0);
						}
					}
				}
			}
			else
				ack->set_result(-1);

			listener_.send_protocol(socket, ack);
		}
		break;

	default:
		break;
	}
}

void CMainDlg::InsterLog( char* pFmt, ... )
{
	char text[32];
	CTime tm = CTime::GetCurrentTime();
	sprintf(text, "%.2d:%.2d:%.2d ", tm.GetHour(), tm.GetMinute(), tm.GetSecond());

	va_list args;
	char buf[1024];
	va_start(args, pFmt);
	vsprintf(buf, pFmt, args);
	log_ = CString(text) + CString(buf) + "\r\n" + log_;
	::SetWindowText( GetDlgItem(IDC_EDT_LOG),  log_);
}

S_INT_32 CMainDlg::FindLstItemBySocket( SOCKET socket)
{
	S_INT_32 count = lstclients_.GetItemCount();
	for ( S_INT_32 i = 0; i < count; i ++ )
	{
		if ( lstclients_.GetItemData( i) == socket)
		{
			return i;
		}
	}

	return -1;
}

S_INT_32 CMainDlg::FindLstItemByName( NS_STL::string robotname)
{
	S_INT_32 count = lstclients_.GetItemCount();
	for ( S_INT_32 i = 0; i < count; i ++ )
	{
		char text[256];

		if ( lstclients_.GetItemText( i, 1, text, sizeof(text)) > 0)
		{
			if ( robotname == text)
				return i;
		}
		else
			break;
	}

	return -1;
}


LRESULT CMainDlg::OnBnClickedBtnStartall(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CSelTypeDlg dlg;
	dlg.DoModal();

	if ( dlg.sel_index_ != "")
	{
		S_INT_32 count = lstclients_.GetItemCount();
		char text[256];
		for ( S_INT_32 i = 0; i < count; i ++ )
		{
			S_UINT_32 data = lstclients_.GetItemData( i);
			if ( data != 0)
			{
				SOCKET socket = (SOCKET)data;
				Robot_Start_Ack* ack = new Robot_Start_Ack();
				if ( ack)
				{
					ack->set_startusernum( -1);
					ack->set_logininterval( 10);
					ack->add_behavior(g_opt.get_testcase(dlg.sel_index_));

					listener_.send_protocol( socket, ack);
					lstclients_.SetItem( i, 0, LVIF_IMAGE, "", ICON_RUNNING, 0, 0, 0);
					lstclients_.GetItemText( i, 1, text, sizeof(text));
					InsterLog( "start command send to '%s'", text);
				}
			}
		}

		::SetFocus( GetDlgItem( IDC_LST_CLIENTS));
	}

	return 0;
}

LRESULT CMainDlg::OnBnClickedBtnStopall(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	S_INT_32 count = lstclients_.GetItemCount();
	char text[256];
	for ( S_INT_32 i = 0; i < count; i ++ )
	{
		S_UINT_32 data = lstclients_.GetItemData( i);
		if ( data != 0)
		{
			SOCKET socket = (SOCKET)data;
			Robot_Stop_Ack* ack = new Robot_Stop_Ack();
			if ( ack)
			{
				listener_.send_protocol( socket, ack);
				lstclients_.SetItem( i, 0, LVIF_IMAGE, "", ICON_CONNECTED, 0, 0, 0);
				lstclients_.GetItemText( i, 1, text, sizeof(text));
				InsterLog( "stop command send to '%s'", text);
			}
		}
	}

	::SetFocus( GetDlgItem( IDC_LST_CLIENTS));

	return 0;
}

LRESULT CMainDlg::OnBnClickedBtnStartSel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if ( lstclients_.GetSelectedCount() == 0) return 0;

	CSelTypeDlg dlg;
	dlg.DoModal();

	if ( dlg.sel_index_ != "")
	{
		S_INT_32 count = lstclients_.GetItemCount();
		char text[256];
		for ( S_INT_32 i = 0; i < count; i ++ )
		{
			if ( LVIS_SELECTED == lstclients_.GetItemState( i, LVIS_SELECTED))
			{
				S_UINT_32 data = lstclients_.GetItemData( i);
				if ( data != 0)
				{
					SOCKET socket = (SOCKET)data;
					Robot_Start_Ack* ack = new Robot_Start_Ack();
					if ( ack)
					{
						ack->set_startusernum( -1);
						ack->set_logininterval( 10);
						ack->add_behavior( g_opt.get_testcase( dlg.sel_index_));
						listener_.send_protocol( socket, ack);
						lstclients_.SetItem( i, 0, LVIF_IMAGE, "", ICON_RUNNING, 0, 0, 0);
						lstclients_.GetItemText( i, 1, text, sizeof(text));
						InsterLog( "start command send to '%s'", text);
					}
				}
			}
		}

		::SetFocus( GetDlgItem( IDC_LST_CLIENTS));
	}

	return 0;
}

LRESULT CMainDlg::OnBnClickedBtnStopSel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	S_INT_32 count = lstclients_.GetItemCount();
	char text[256];
	for ( S_INT_32 i = 0; i < count; i ++ )
	{
		if ( LVIS_SELECTED == lstclients_.GetItemState( i, LVIS_SELECTED))
		{
			S_UINT_32 data = lstclients_.GetItemData( i);
			if ( data != 0)
			{
				SOCKET socket = (SOCKET)data;
				Robot_Stop_Ack* ack = new Robot_Stop_Ack();
				if ( ack)
				{
					listener_.send_protocol( socket, ack);
					lstclients_.SetItem( i, 0, LVIF_IMAGE, "", ICON_CONNECTED, 0, 0, 0);
					lstclients_.GetItemText( i, 1, text, sizeof(text));
					InsterLog( "stop command send to '%s'", text);
				}
			}
		}
	}

	::SetFocus( GetDlgItem( IDC_LST_CLIENTS));

	return 0;
}
