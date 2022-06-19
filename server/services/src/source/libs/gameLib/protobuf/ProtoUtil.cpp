#include <gameLib/protobuf/ProtoUtil.h>

#include <gameLib/gatehome/ProtoTokenUtil.h>

using namespace google::protobuf;

bool ProtoUtil::get_msgiid(BasicProtocol* msg, S_UINT_16& msgiid)
{
	const FieldDescriptor * pfield = msg->GetDescriptor()->FindFieldByName("msg_iid");
	if (pfield == 0 || pfield->type() != FieldDescriptor::TYPE_ENUM || pfield->is_optional() == false)
		return false;

	const Reflection* pref = msg->GetReflection();
	if (pref == 0)
		return false;

	msgiid = (S_UINT_16)pref->GetEnumValue(*msg, pfield);

	return true;
}

bool ProtoUtil::get_slot_from_tokenx(BasicProtocol* msg, int& slot)
{
	S_INT_64 giduid = 0, slottoken = 0;
	if (!get_usertokenx(msg, giduid, slottoken))
		return false;

	ProtoTokenUtil::parse_usertoken2(slottoken, slot);

	return true;
}

bool ProtoUtil::get_gate_from_token(BasicProtocol* msg, S_INT_64& gateid)
{
	S_INT_64 giduid = 0, slottoken = 0;
	if (!get_usertokenx(msg, giduid, slottoken))
		return false;

	ProtoTokenUtil::parse_usergate3( giduid, gateid);

	return true;
}

bool ProtoUtil::get_useriid_from_token(BasicProtocol* msg, S_INT_64& uid)
{
	S_INT_64 giduid = 0, slottoken = 0;
	if (!get_usertokenx(msg, giduid, slottoken))
		return false;

	ProtoTokenUtil::parse_usergate2(giduid, uid);

	return true;
}

bool ProtoUtil::get_stuidtkm_from_tokenx(BasicProtocol* msg, int& slot, S_INT_64& useriid, S_INT_64& tks)
{
	S_INT_64 giduid = 0;
	if (!get_usertokenx(msg, giduid, tks))
		return false;

	ProtoTokenUtil::parse_usertoken2(tks, slot);
	ProtoTokenUtil::parse_usergate2(giduid, useriid);

	return true;
}

bool ProtoUtil::get_usertokenx(BasicProtocol* msg, S_INT_64& giduid, S_INT_64& slottoken)
{
	const FieldDescriptor * pfield = msg->GetDescriptor()->FindFieldByName("utoken");
	if (pfield == 0 || pfield->type() != FieldDescriptor::TYPE_MESSAGE || pfield->is_optional() == false)
		return false;
	const Descriptor *pu = pfield->message_type();
	const Descriptor *pus = PRO::UserToken::GetDescriptor();
	if (pu != pus)
		return false;

	const Reflection* pref = msg->GetReflection();
	if (pref == 0)
		return false;

	Message* umsg = pref->MutableMessage( msg, pfield);
	if (umsg == 0)
		return false;
	PRO::UserToken *pt = dynamic_cast<PRO::UserToken*>(umsg);
	if (pt == 0)
		return false;

	giduid = pt->giduid();
	slottoken = pt->slottoken();

	return true;
}

bool ProtoUtil::set_usertokenx(BasicProtocol* msg, S_INT_64 giduid, S_INT_64 slottoken)
{
	const FieldDescriptor * pfield = msg->GetDescriptor()->FindFieldByName("utoken");
	if (pfield == 0 || pfield->type() != FieldDescriptor::TYPE_MESSAGE || pfield->is_optional() == false)
		return false;
	const Descriptor *pu = pfield->message_type();
	const Descriptor *pus = PRO::UserToken::GetDescriptor();
	if (pu != pus)
		return false;

	const Reflection* pref = msg->GetReflection();
	if (pref == 0)
		return false;

	PRO::UserToken *pt = new PRO::UserToken();
	pt->set_giduid(giduid);
	pt->set_slottoken(slottoken);
	pref->SetAllocatedMessage(msg, pt, pfield);

	return true;
}
