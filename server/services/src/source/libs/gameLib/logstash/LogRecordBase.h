#ifndef __LOGRECORDBASE_H__
#define __LOGRECORDBASE_H__

#include <string>
#include <cmsLib/core_type.h>
#include <gameLib/LogExt.h>

class LogRecordBase
{
public:
	LogRecordBase();

	void save();

protected:
	virtual bool to_json( std::string& jstr) = 0;
	virtual std::string get_log_cat() = 0;

protected:
	S_TIMESTAMP		now_;
};

#endif //__LOGRECORDBASE_H__
