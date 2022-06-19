#include "StdAfx.h"

#include "CommandTestImpl.h"
#include <cmsLib/GlobalSettings.h>
#include <cmsLib/util/ShareUtil.h>

USE_PROTOCOL_NAMESPACE;

void CommandTestImpl::pet_adoptone(S_INT_64 petid)
{
	ret_desc_ = "";
	if (!islogon())
	{
		ret_desc_ = "用户未登陆\r\n";
		return;
	}

	Pet_AdoptOne_req *req = new Pet_AdoptOne_req();
	req->set_pet_iid(petid);

	if (!send_to_gts(req))
	{
		ret_desc_ = "发送协议失败\r\n";
		return;
	}
}

void CommandTestImpl::on_pet_adoptone_ack(BasicProtocol* pro, CString* pRetMsg)
{
	Pet_AdoptOne_ack* ack = dynamic_cast<Pet_AdoptOne_ack*>(pro);

	CString str1;
	str1.Format("领养宠物 返回 result:%d \r\n", ack->result());
	*pRetMsg += str1;

	if (ack->has_pet())
	{
		str1.Format("宠物 iid:%lld ver:%d resid:%lld \r\n", ack->pet().mypet_iid(), 
			ack->pet().ver_(), ack->pet().pet_iid());
		*pRetMsg += str1;
	}
}

void CommandTestImpl::pet_releaseone(S_INT_64 mypetiid)
{
	ret_desc_ = "";
	if (!islogon())
	{
		ret_desc_ = "用户未登陆\r\n";
		return;
	}

	Pet_ReleaseOne_req *req = new Pet_ReleaseOne_req();
	req->set_mypet_iid(mypetiid);

	if (!send_to_gts(req))
	{
		ret_desc_ = "发送协议失败\r\n";
		return;
	}
}

void CommandTestImpl::on_pet_releaseone_ack(BasicProtocol* pro, CString* pRetMsg)
{
	Pet_ReleaseOne_ack *ack = dynamic_cast<Pet_ReleaseOne_ack*>(pro);

	CString str1;
	str1.Format("弃养宠物:%lld 返回 result:%d \r\n", ack->mypet_iid(), ack->result());
	*pRetMsg += str1;
}
