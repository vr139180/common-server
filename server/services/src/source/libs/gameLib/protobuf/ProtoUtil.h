#ifndef __PROTOUTIL_H__
#define __PROTOUTIL_H__

#include <gameLib/protobuf/Proto_all.h>

class ProtoUtil
{
private:
	ProtoUtil() {}

public:
	//通过反射获取usertoken字段
	static bool get_usertokenx(BasicProtocol* msg, S_INT_64& giduid, S_INT_64& slottoken);
	static bool set_usertokenx(BasicProtocol* msg, S_INT_64 giduid, S_INT_64 slottoken);

	static bool get_slot_from_tokenx(BasicProtocol* msg, int& slot);
	static bool get_stuidtkm_from_tokenx(BasicProtocol* msg, int& slot, S_INT_64& useriid, S_INT_64& tks);
	static bool get_gate_from_token(BasicProtocol* msg, S_INT_64& gateid);
	static bool get_useriid_from_token(BasicProtocol* msg, S_INT_64& uid);

	static bool get_msgiid(BasicProtocol* msg, S_UINT_16& msgiid);
};

#endif //__PROTOUTIL_H__
