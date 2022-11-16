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

#include "lobby/LobbyService.h"

#include <gameLib/protobuf/Proto_all.h>
#include <gameLib/LogExt.h>

#include "HomeServiceApp.h"

USE_PROTOCOL_NAMESPACE

void LobbyService::InitNetMessage()
{
	REGISTERMSG(USER_PROTYPE::USER_LOGOUT_NTF, &LobbyService::on_lb_userlogout_ntf, this);

	REGISTERMSG(USER_PROTYPE::USER_ROLELIST_REQ, &LobbyService::on_lb_rolelist_req, this);
	REGISTERMSG(USER_PROTYPE::USER_ROLECREATE_REQ, &LobbyService::on_lb_rolecreate_req, this);
	REGISTERMSG(USER_PROTYPE::USER_ROLESELECT_REQ, &LobbyService::on_lb_roleselect_req, this);

	REGISTERMSG(BUILD_PROTYPE::BUILD_ADDITEM_REQ, &LobbyService::on_lb_build_additem_req, this);
	REGISTERMSG(BUILD_PROTYPE::BUILD_DELITEM_REQ, &LobbyService::on_lb_build_delitem_req, this);

	REGISTERMSG(PET_PROTYPE::PET_ADOPTONE_REQ, &LobbyService::on_lb_pet_adoptone_req, this);
	REGISTERMSG(PET_PROTYPE::PET_RELEASEONE_REQ, &LobbyService::on_lb_pet_releaseone_req, this);

	REGISTERMSG(TASK_PROTYPE::TASK_WAITLIST_REQ, &LobbyService::on_lb_task_waitlist_req, this);
	REGISTERMSG(TASK_PROTYPE::TASK_MYTASKLIST_REQ, &LobbyService::on_lb_task_mytasks_req, this);
	REGISTERMSG(TASK_PROTYPE::TASK_GETTASK_REQ, &LobbyService::on_lb_task_get_req, this);
	REGISTERMSG(TASK_PROTYPE::TASK_SUBMITTASK_REQ, &LobbyService::on_lb_task_submit_req, this);
	REGISTERMSG(TASK_PROTYPE::TASK_GIVEUPTASK_REQ, &LobbyService::on_lb_task_giveup_req, this);
}

void LobbyService::on_lb_userlogout_ntf(NetProtocol* pro, bool& autorelease)
{
	LobbyUser *puser = get_userofsame_from_msg(pro);
	if (puser == 0) return;

	logDebug(out_runtime, "recv user:%lld logount request", puser->get_user_iid());
	puser->rest_user();
}

void LobbyService::on_lb_rolelist_req(NetProtocol* pro, bool& autorelease)
{
	LobbyUser *puser = get_userofsame_from_msg(pro);
	if (puser == 0) return;

	User_RoleCreate_req* req = dynamic_cast<User_RoleCreate_req*>(pro->msg_);

	puser->on_ls_rolecreate_req(req->nickname().c_str());
}

void LobbyService::on_lb_rolecreate_req(NetProtocol* pro, bool& autorelease)
{
	LobbyUser *puser = get_userofsame_from_msg(pro);
	if (puser == 0) return;

	User_RoleCreate_req* req = dynamic_cast<User_RoleCreate_req*>(pro->msg_);

	puser->on_ls_rolecreate_req(req->nickname().c_str());
}

void LobbyService::on_lb_roleselect_req(NetProtocol* pro, bool& autorelease)
{
	LobbyUser *puser = get_userofsame_from_msg(pro);
	if (puser == 0) return;
	User_RoleSelect_req* req = dynamic_cast<User_RoleSelect_req*>(pro->msg_);

	puser->on_ls_roleselect_req(req->role_iid());
}

void LobbyService::on_lb_build_additem_req(NetProtocol* pro, bool& autorelease)
{
	LobbyUser *puser = get_userofsame_from_msg(pro);
	if (puser == 0) return;
	Build_AddItem_req* req = dynamic_cast<Build_AddItem_req*>(pro->msg_);

	S_INT_32 ret = 0;
	PRO::DBUserHomeStructureItem* pitem = puser->build_additem(req->parent_building(), req->building_resid(), req->look_at().c_str(),
		req->building_pos().c_str(), ret);

	PRO::Build_AddItem_ack *ack = new PRO::Build_AddItem_ack();
	ack->set_result(ret);

	if (pitem != 0)
		ack->mutable_item()->CopyFrom(*pitem);

	svrApp.send_protocol_to_gate(ack);
}

void LobbyService::on_lb_build_delitem_req(NetProtocol* pro, bool& autorelease)
{
	LobbyUser *puser = get_userofsame_from_msg(pro);
	if (puser == 0) return;
	Build_DelItem_req* req = dynamic_cast<Build_DelItem_req*>(pro->msg_);

	S_INT_32 ret = puser->build_delitem(req->building_iid());
	Build_DelItem_ack *ack = new Build_DelItem_ack();

	ack->set_building_iid(req->building_iid());
	ack->set_result(ret);

	svrApp.send_protocol_to_gate(ack);
}

void LobbyService::on_lb_pet_adoptone_req(NetProtocol* pro, bool& autorelease)
{
	LobbyUser *puser = get_userofsame_from_msg(pro);
	if (puser == 0) return;
	Pet_AdoptOne_req* req = dynamic_cast<Pet_AdoptOne_req*>(pro->msg_);

	S_INT_32 ret = 0;
	PRO::DBUserPetItem *pitem = puser->pet_adoptone(req->pet_iid(), ret);

	PRO::Pet_AdoptOne_ack *ack = new PRO::Pet_AdoptOne_ack();
	ack->set_result(ret);
	if (pitem != 0)
		ack->mutable_pet()->CopyFrom(*pitem);

	svrApp.send_protocol_to_gate(ack);
}

void LobbyService::on_lb_pet_releaseone_req(NetProtocol* pro, bool& autorelease)
{
	LobbyUser *puser = get_userofsame_from_msg(pro);
	if (puser == 0) return;
	Pet_ReleaseOne_req* req = dynamic_cast<Pet_ReleaseOne_req*>(pro->msg_);

	S_INT_32 ret = puser->pet_releaseone(req->mypet_iid());

	Pet_ReleaseOne_ack *ack = new Pet_ReleaseOne_ack();

	ack->set_mypet_iid(req->mypet_iid());
	ack->set_result(ret);

	svrApp.send_protocol_to_gate(ack);
}

void LobbyService::on_lb_task_waitlist_req(NetProtocol* pro, bool& autorelease)
{
	LobbyUser *puser = get_userofsame_from_msg(pro);
	if (puser == 0) return;
	
	BasicProtocol* ack = puser->task_get_waitlist();
	svrApp.send_protocol_to_gate(ack);
}

void LobbyService::on_lb_task_mytasks_req(NetProtocol* pro, bool& autorelease)
{
	LobbyUser *puser = get_userofsame_from_msg(pro);
	if (puser == 0) return;
	
	BasicProtocol* ack = puser->task_get_mytasks();

	svrApp.send_protocol_to_gate(ack);
}

void LobbyService::on_lb_task_get_req(NetProtocol* pro, bool& autorelease)
{
	LobbyUser *puser = get_userofsame_from_msg(pro);
	if (puser == 0) return;

	Task_GetTask_req* req = dynamic_cast<Task_GetTask_req*>(pro->msg_);
	BasicProtocol* ack = puser->task_get_from_waitlist( req->task_iid());

	svrApp.send_protocol_to_gate(ack);
}

void LobbyService::on_lb_task_submit_req(NetProtocol* pro, bool& autorelease)
{
	LobbyUser *puser = get_userofsame_from_msg(pro);
	if (puser == 0) return;

	Task_SubmitTask_req* req = dynamic_cast<Task_SubmitTask_req*>(pro->msg_);
	BasicProtocol* ack = puser->task_submit_one(req->task_iid());

	svrApp.send_protocol_to_gate(ack);
}

void LobbyService::on_lb_task_giveup_req(NetProtocol* pro, bool& autorelease)
{
	LobbyUser *puser = get_userofsame_from_msg(pro);
	if (puser == 0) return;

	Task_GiveupTask_req* req = dynamic_cast<Task_GiveupTask_req*>(pro->msg_);
	BasicProtocol* ack = puser->task_giveup_task(req->task_iid());

	svrApp.send_protocol_to_gate(ack);
}
