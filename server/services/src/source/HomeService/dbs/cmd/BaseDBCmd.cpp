#include "dbs/cmd/BaseDBCmd.h"

#include "lobby/LobbyService.h"

BaseDBCmd::BaseDBCmd(S_INT_64 uid, S_INT_64 token, LobbyService* p):user_iid_(uid)
,protoken_( token)
,lobby_( p)
{
}

void BaseDBCmd::dispath_again()
{
	lobby_->regist_syscmd(this);
}