#include <gameLib/gatehome/ProtoTokenUtil.h>

S_INT_64 ProtoTokenUtil::build_usertoken(S_INT_64 slot, S_INT_64 token)
{
	S_INT_64 r = 0;
	MAKE_USERTOKEN(slot, token, r);
	return r;
}

void ProtoTokenUtil::parse_usertoken(S_INT_64 utoken, int& slot, S_INT_64& token)
{
	PARSE_USERTOKEN(utoken, slot, token);
}

void ProtoTokenUtil::parse_usertoken2(S_INT_64 utoken, int& slot)
{
	S_INT_64 token = 0;
	PARSE_USERTOKEN(utoken, slot, token);
}

S_INT_64 ProtoTokenUtil::build_usergate(S_INT_64 gateiid, S_INT_64 useriid)
{
	S_INT_64 r = 0;
	MAKE_USERGATE(gateiid, useriid, r);
	return r;
}

void ProtoTokenUtil::parse_usergate(S_INT_64 giduid, S_INT_64& gateid, S_INT_64& useriid)
{
	PARSE_USERGATE(giduid, gateid, useriid);
}

void ProtoTokenUtil::parse_usergate2(S_INT_64 giduid, S_INT_64& useriid)
{
	S_INT_64 gateid = 0;
	PARSE_USERGATE(giduid, gateid, useriid);
}

void ProtoTokenUtil::parse_usergate3(S_INT_64 giduid, S_INT_64& gateid)
{
	S_INT_64 useriid = 0;
	PARSE_USERGATE(giduid, gateid, useriid);
}
