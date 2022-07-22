#include "lobby/LobbyService.h"

#include <gameLib/protobuf/Proto_all.h>
#include <gameLib/protobuf/ProtoUtil.h>
#include <gameLib/LogExt.h>

#include "HomeServiceApp.h"

USE_PROTOCOL_NAMESPACE

void LobbyService::InitNetMessage()
{
	REGISTERMSG(GATEHOME_PROTYPE::GHS_USERINIT_REQ, &LobbyService::on_lb_ghuserinit_req, this);
	REGISTERMSG(GATEHOME_PROTYPE::GHS_USERLOGOUT_NTF, &LobbyService::on_lb_ghuserlogout_ntf, this);

	REGISTERMSG(USER_PROTYPE::USER_ROLECREATE_REQ, &LobbyService::on_lb_rolecreate_req, this);
	REGISTERMSG(USER_PROTYPE::USER_ROLESELECT_REQ, &LobbyService::on_lb_roleselect_req, this);
	REGISTERMSG(GATEHOME_PROTYPE::GHS_ROLEDETAIL_ASK, &LobbyService::on_lb_ghroledetail_ask, this);

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

void LobbyService::on_lb_ghuserinit_req(BasicProtocol* pro, bool& autorelease)
{
	GHS_UserInit_req * req = dynamic_cast<GHS_UserInit_req*>(pro);

	LobbyUser *puser = get_userbyslot_from_msg(pro);
	if (puser == 0)
		return;
	const PRO::UserToken& ut = req->utoken();

	puser->init_user(ut.giduid(), ut.slottoken());

	logDebug(out_net, "recv user:%lld init request", puser->get_user_iid());
}

void LobbyService::on_lb_ghuserlogout_ntf(BasicProtocol* pro, bool& autorelease)
{
	LobbyUser *puser = get_userofsame_from_msg(pro);
	if (puser == 0) return;

	logDebug(out_net, "recv user:%lld logount request", puser->get_user_iid());
	puser->rest_user();
}

void LobbyService::on_lb_rolecreate_req(BasicProtocol* pro, bool& autorelease)
{
	LobbyUser *puser = get_userofsame_from_msg(pro);
	if (puser == 0) return;

	User_RoleCreate_req* req = dynamic_cast<User_RoleCreate_req*>(pro);

	puser->on_ls_rolecreate_req(req->nickname().c_str());
}

void LobbyService::on_lb_roleselect_req(BasicProtocol* pro, bool& autorelease)
{
	LobbyUser *puser = get_userofsame_from_msg(pro);
	if (puser == 0) return;
	User_RoleSelect_req* req = dynamic_cast<User_RoleSelect_req*>(pro);

	puser->on_ls_roleselect_req(req->role_iid());
}

void LobbyService::on_lb_ghroledetail_ask(BasicProtocol* pro, bool& autorelease)
{
	LobbyUser *puser = get_userofsame_from_msg(pro);
	if (puser == 0) return;

	puser->notify_roledetail_to_user();
}

void LobbyService::on_lb_build_additem_req(BasicProtocol* pro, bool& autorelease)
{
	LobbyUser *puser = get_userofsame_from_msg(pro);
	if (puser == 0) return;
	Build_AddItem_req* req = dynamic_cast<Build_AddItem_req*>(pro);

	S_INT_32 ret = 0;
	PRO::DBUserHomeStructureItem* pitem = puser->build_additem(req->parent_building(), req->building_resid(), req->look_at().c_str(),
		req->building_pos().c_str(), ret);

	PRO::Build_AddItem_ack *ack = new PRO::Build_AddItem_ack();
	puser->set_usertoken(ack);
	ack->set_result(ret);

	if (pitem != 0)
		ack->mutable_item()->CopyFrom(*pitem);

	svrApp.send_protocol_to_gate(ack);
}

void LobbyService::on_lb_build_delitem_req(BasicProtocol* pro, bool& autorelease)
{
	LobbyUser *puser = get_userofsame_from_msg(pro);
	if (puser == 0) return;
	Build_DelItem_req* req = dynamic_cast<Build_DelItem_req*>(pro);

	S_INT_32 ret = puser->build_delitem(req->building_iid());
	Build_DelItem_ack *ack = new Build_DelItem_ack();
	puser->set_usertoken(ack);

	ack->set_building_iid(req->building_iid());
	ack->set_result(ret);

	svrApp.send_protocol_to_gate(ack);
}

void LobbyService::on_lb_pet_adoptone_req(BasicProtocol* pro, bool& autorelease)
{
	LobbyUser *puser = get_userofsame_from_msg(pro);
	if (puser == 0) return;
	Pet_AdoptOne_req* req = dynamic_cast<Pet_AdoptOne_req*>(pro);

	S_INT_32 ret = 0;
	PRO::DBUserPetItem *pitem = puser->pet_adoptone(req->pet_iid(), ret);

	PRO::Pet_AdoptOne_ack *ack = new PRO::Pet_AdoptOne_ack();
	puser->set_usertoken(ack);
	ack->set_result(ret);
	if (pitem != 0)
		ack->mutable_pet()->CopyFrom(*pitem);

	svrApp.send_protocol_to_gate(ack);
}

void LobbyService::on_lb_pet_releaseone_req(BasicProtocol* pro, bool& autorelease)
{
	LobbyUser *puser = get_userofsame_from_msg(pro);
	if (puser == 0) return;
	Pet_ReleaseOne_req* req = dynamic_cast<Pet_ReleaseOne_req*>(pro);

	S_INT_32 ret = puser->pet_releaseone(req->mypet_iid());

	Pet_ReleaseOne_ack *ack = new Pet_ReleaseOne_ack();
	puser->set_usertoken(ack);

	ack->set_mypet_iid(req->mypet_iid());
	ack->set_result(ret);

	svrApp.send_protocol_to_gate(ack);
}

void LobbyService::on_lb_task_waitlist_req(BasicProtocol* pro, bool& autorelease)
{
	LobbyUser *puser = get_userofsame_from_msg(pro);
	if (puser == 0) return;
	
	BasicProtocol* ack = puser->task_get_waitlist();
	puser->set_usertoken(ack);
	svrApp.send_protocol_to_gate(ack);
}

void LobbyService::on_lb_task_mytasks_req(BasicProtocol* pro, bool& autorelease)
{
	LobbyUser *puser = get_userofsame_from_msg(pro);
	if (puser == 0) return;
	
	BasicProtocol* ack = puser->task_get_mytasks();
	puser->set_usertoken(ack);
	svrApp.send_protocol_to_gate(ack);
}

void LobbyService::on_lb_task_get_req(BasicProtocol* pro, bool& autorelease)
{
	LobbyUser *puser = get_userofsame_from_msg(pro);
	if (puser == 0) return;

	Task_GetTask_req* req = dynamic_cast<Task_GetTask_req*>(pro);
	BasicProtocol* ack = puser->task_get_from_waitlist( req->task_iid());
	puser->set_usertoken(ack);
	svrApp.send_protocol_to_gate(ack);
}

void LobbyService::on_lb_task_submit_req(BasicProtocol* pro, bool& autorelease)
{
	LobbyUser *puser = get_userofsame_from_msg(pro);
	if (puser == 0) return;

	Task_SubmitTask_req* req = dynamic_cast<Task_SubmitTask_req*>(pro);
	BasicProtocol* ack = puser->task_submit_one(req->task_iid());
	puser->set_usertoken(ack);
	svrApp.send_protocol_to_gate(ack);
}

void LobbyService::on_lb_task_giveup_req(BasicProtocol* pro, bool& autorelease)
{
	LobbyUser *puser = get_userofsame_from_msg(pro);
	if (puser == 0) return;

	Task_GiveupTask_req* req = dynamic_cast<Task_GiveupTask_req*>(pro);
	BasicProtocol* ack = puser->task_giveup_task(req->task_iid());
	puser->set_usertoken(ack);
	svrApp.send_protocol_to_gate(ack);
}
