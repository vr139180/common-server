#ifndef __GAMEDATALOG_H__
#define __GAMEDATALOG_H__

#include <gameLib/logstash/LogRecordBase.h>

class GameDataLog : public LogRecordBase
{
public:
	GameDataLog();

protected:
	virtual bool to_json(std::string& jstr);
	virtual std::string get_log_cat() { return out_gamedata; }

};

#endif //__GAMEDATALOG_H__
