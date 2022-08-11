#ifndef __DAEMONNODE_H__
#define __DAEMONNODE_H__

#include <cmsLib/prolib/core_type.h>
#include <cmsLib/util/XmlUtil.h>
#include <gameLib/protobuf/Proto_all.h>

#include <string>
#include <map>
#include <list>

class VirtualUser;
class TestCaseWorkFlow;

class DaemonNode
{
public:
	DaemonNode();
	virtual ~DaemonNode();

	void link_after_build( VirtualUser* v, TestCaseWorkFlow* f);

public:
	virtual void init_before_start();

	virtual void update( ULONGLONG now){}

	virtual void handle_message(S_UINT_16 proid, BasicProtocol* pro, bool& handle);

protected:
	virtual void update_do( ULONGLONG now){}

protected:
	VirtualUser*		vuser_;
	TestCaseWorkFlow*	wf_;
};

#endif	//__DAEMONNODE_H__
