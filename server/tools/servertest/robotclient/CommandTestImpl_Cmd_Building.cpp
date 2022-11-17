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

#include "StdAfx.h"

#include "CommandTestImpl.h"
#include <cmsLib/GlobalSettings.h>
#include <cmsLib/util/ShareUtil.h>

USE_PROTOCOL_NAMESPACE;

void CommandTestImpl::build_additem(S_INT_64 parent, S_INT_64 resid)
{
	ret_desc_ = "";
	if (!islogon())
	{
		ret_desc_ = "用户未登陆\r\n";
		return;
	}

	Build_AddItem_req *req = new Build_AddItem_req();
	req->set_parent_building(parent);
	req->set_building_resid(resid);
	req->set_look_at("{1,1,1}");
	req->set_building_pos("{2,2,2}");

	if (!send_to_gts(req))
	{
		ret_desc_ = "发送协议失败\r\n";
		return;
	}
}

void CommandTestImpl::on_build_additem_ack(BasicProtocol* pro, CString* pRetMsg)
{
	Build_AddItem_ack* ack = dynamic_cast<Build_AddItem_ack*>(pro);

	CString str1;
	str1.Format("增加建筑 返回 result:%d \r\n", ack->result());
	*pRetMsg += str1;

	if (ack->has_item())
	{
		str1.Format("建筑 iid:%lld ver:%d resid:%lld \r\n", ack->item().building_iid(), 
			ack->item().ver_(), ack->item().building_resid());
		*pRetMsg += str1;
	}
}

void CommandTestImpl::build_delitem(S_INT_64 buildiid)
{
	ret_desc_ = "";
	if (!islogon())
	{
		ret_desc_ = "用户未登陆\r\n";
		return;
	}

	Build_DelItem_req *req = new Build_DelItem_req();
	req->set_building_iid(buildiid);

	if (!send_to_gts(req))
	{
		ret_desc_ = "发送协议失败\r\n";
		return;
	}
}

void CommandTestImpl::on_build_delitem_ack(BasicProtocol* pro, CString* pRetMsg)
{
	Build_DelItem_ack *ack = dynamic_cast<Build_DelItem_ack*>(pro);

	CString str1;
	str1.Format("删除建筑:%lld 返回 result:%d \r\n", ack->building_iid(), ack->result());
	*pRetMsg += str1;
}
