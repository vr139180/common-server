#include "StdAfx.h"

#include "CommandTestImpl.h"
#include <cmsLib/GlobalSettings.h>
#include <cmsLib/util/ShareUtil.h>
#include <cmsLib/encryption/md5/md5.h>

USE_PROTOCOL_NAMESPACE;

void CommandTestImpl::ping()
{
    ret_desc_ ="";
    if( socket2_ == INVALID_SOCKET)
    {
        if( !connect_to_gts())
        {
            ret_desc_ ="gts socket打开错误，服务器可能没有运行！\r\n";
            return;
        }
    }

    //Client_Ping_Ntf *req =new Client_Ping_Ntf();

    startThread();

	/*
    if( !send_to_gts( req))
    {
        ret_desc_ ="发送Ping协议失败\r\n";
        return;
    }
	*/
}

void CommandTestImpl::on_ping_ntf(BasicProtocol* pro, CString* pRetMsg)
{
	pRetMsg->Format("Ping 测试回包");
	return;
}

void CommandTestImpl::login( const char* name, const char* pwd)
{
	ret_desc_ ="";

	if( socket_ == INVALID_SOCKET)
	{
		if( !connect_to_lgs())
		{
			ret_desc_ ="lgs socket打开错误，服务器可能没有运行！\r\n";
			return;
		}
	}

	username_ =name;

	//do lgs
	thread_step_ =1;

	startThread();

	User_Login_req *req =new User_Login_req();
	req->set_type(1);
	req->set_account(name);
	req->set_pwd(md5(pwd).c_str());

	if( !send_to_lgs( req))
	{
		ret_desc_ ="发送协议失败\r\n";
		return;
	}
}

void CommandTestImpl::logintoken()
{
	ret_desc_ = "";

	if (socket_ == INVALID_SOCKET)
	{
		if (!connect_to_lgs())
		{
			ret_desc_ = "lgs socket打开错误，服务器可能没有运行！\r\n";
			return;
		}
	}

	//do lgs
	thread_step_ = 1;

	startThread();

	User_Login_req *req = new User_Login_req();
	req->set_type(2);
	req->set_token(ut.c_str());
	req->set_userid(uid);

	if (!send_to_lgs(req))
	{
		ret_desc_ = "发送协议失败\r\n";
		return;
	}
}

void CommandTestImpl::on_login_ack( BasicProtocol* pro, CString* pRetMsg)
{
	User_ProxyLogin_ack* ack =dynamic_cast<User_ProxyLogin_ack*>(pro);

	if( ack->result() != 0)
	{
		pRetMsg->Format( "Gate验证失败....result:[%d]\r\n", ack->result());
		disconnect_to_gts();

		thread_step_ =0;

		return;
	}

	CString str1;
	str1.Format( "Gate验证成功 userid:%lld \r\n", userid_);
	*pRetMsg += str1;

	*pRetMsg += "\r\n";
}

void CommandTestImpl::logout()
{
	ret_desc_ ="";
	if( !islogon())
	{
		ret_desc_ ="用户未登陆\r\n";
		return;
	}

	User_Logout_ntf *req =new User_Logout_ntf();
	if( !send_to_gts( req))
	{
		ret_desc_ ="发送协议失败\r\n";
		return;
	}
}

void CommandTestImpl::on_rolelist_ack(BasicProtocol* pro, CString* pRetMsg)
{
	PRO::User_RoleList_ack *ack = dynamic_cast<User_RoleList_ack*>(pro);
	CString str1;
	if (ack->has_roles())
	{
		str1.Format(" RoleList size:%d \r\n", ack->roles().roles_size());
		*pRetMsg += str1;

		for (int ii = 0; ii < ack->roles().roles_size(); ++ii)
		{
			const DBRoleBaseInfo& r = ack->roles().roles(ii);
			str1.Format("role iid:%lld nickname:%s ver:%d \r\n", r.role_iid(), r.nickname().c_str(), r.ver_());
			*pRetMsg += str1;
		}
	}
	else
	{
		*pRetMsg += " RoleList size:0 \r\n";
	}
}

void CommandTestImpl::roleselect(S_INT_64 rid)
{
	ret_desc_ = "";
	if (!islogon())
	{
		ret_desc_ = "用户未登陆\r\n";
		return;
	}

	User_RoleSelect_req *req = new User_RoleSelect_req();
	req->set_role_iid(rid);
	if (!send_to_gts(req))
	{
		ret_desc_ = "发送协议失败\r\n";
		return;
	}
}

void CommandTestImpl::on_roleselect_ack(BasicProtocol* pro, CString* pRetMsg)
{
	User_RoleSelect_ack *ack = dynamic_cast<User_RoleSelect_ack*>(pro);

	CString str1;
	str1.Format("选择角色 返回 result:%d roleid:%lld \r\n", ack->result(), ack->role_iid());
	*pRetMsg += str1;

	if (ack->result() == 0)
	{
		this->role_iid_ = ack->role_iid();
	}
}

void CommandTestImpl::rolecreate(const char* nickname)
{
	ret_desc_ = "";
	if (!islogon())
	{
		ret_desc_ = "用户未登陆\r\n";
		return;
	}

	User_RoleCreate_req *req = new User_RoleCreate_req();
	req->set_nickname(nickname);
	if (!send_to_gts(req))
	{
		ret_desc_ = "发送协议失败\r\n";
		return;
	}
}

void CommandTestImpl::on_rolecreate_ack(BasicProtocol* pro, CString* pRetMsg)
{
	User_RoleCreate_ack *ack = dynamic_cast<User_RoleCreate_ack*>(pro);

	CString str1;
	str1.Format("创建角色 返回 result:%d roleid:%lld \r\n", ack->result(), ack->role_iid());
	*pRetMsg += str1;
}

void CommandTestImpl::on_roledetaila_ack(BasicProtocol* pro, CString* pRetMsg)
{
	User_RoleDetailA_ntf *ntf = dynamic_cast<User_RoleDetailA_ntf*>(pro);

	CString str1;
	str1.Format("角色 home信息 name:%s \r\n", ntf->home().home_name().c_str());
	*pRetMsg += str1;
	if (ntf->has_homeitems())
	{
		str1.Format("角色 homeitem size:%d \r\n", ntf->homeitems().items_size());
		*pRetMsg += str1;
		for (int ii = 0; ii < ntf->homeitems().items_size(); ++ii)
		{
			const DBUserHomeStructureItem& r = ntf->homeitems().items(ii);
			str1.Format("building iid:%lld ver:%d \r\n", r.building_iid(), r.ver_());
			*pRetMsg += str1;
		}
	}
	else
	{
		str1 = "角色 homeitem size:0 \r\n";
		*pRetMsg += str1;
	}

	str1 = "-----------角色------begin\r\n";
	*pRetMsg += str1;
}

void CommandTestImpl::on_roledetailb_ack(BasicProtocol* pro, CString* pRetMsg)
{
	User_RoleDetailB_ntf *ntf = dynamic_cast<User_RoleDetailB_ntf*>(pro);

	CString str1;
	if (ntf->has_pets())
	{
		str1.Format("角色 宠物信息 size:%d \r\n", ntf->pets().pets_size());
		*pRetMsg += str1;
		for (int ii = 0; ii < ntf->pets().pets_size(); ++ii)
		{
			const DBUserPetItem& r = ntf->pets().pets(ii);
			str1.Format("pet iid:%lld ver:%d \r\n", r.mypet_iid(), r.ver_());
			*pRetMsg += str1;
		}
	}
	else
	{
		str1 = "角色 宠物信息 size:0 \r\n";
		*pRetMsg += str1;
	}
}

void CommandTestImpl::on_roledetailend_ack(BasicProtocol* pro, CString* pRetMsg)
{
	User_RoleDetailEnd_ntf *ntf = dynamic_cast<User_RoleDetailEnd_ntf*>(pro);

	CString str1 = "-----------角色------end\r\n";
	*pRetMsg += str1;
}
