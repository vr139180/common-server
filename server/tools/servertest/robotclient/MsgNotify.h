#pragma once

#include <cmsLib/prolib/core_type.h>

class MsgNotify
{
public:
	MsgNotify(void);
	virtual ~MsgNotify(void);

public:
	S_INT_32	userid_;
	CString		msg_;
};
