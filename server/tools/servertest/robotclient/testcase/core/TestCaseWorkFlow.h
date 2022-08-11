#ifndef __TESTCASEWORKFLOW_H__
#define __TESTCASEWORKFLOW_H__

#include <cmsLib/prolib/core_type.h>
#include <cmsLib/prolib/BasicProtocol.h>

#include <string>
#include <map>

#include "MsgNotify.h"

#include "testcase/core/FlowNode.h"
#include "testcase/core/DaemonNode.h"

class VirtualUser;
class TestCase;

class TestCaseWorkFlow
{
public:
	typedef enum enum_WF_Status
	{
		WFSTATUS_NONE =0,
		WFSTATUS_RUNNING,
		WFSTATUS_STOP
	}WF_STATUS;

public:
	TestCaseWorkFlow();
	virtual ~TestCaseWorkFlow();

	void init( TestCase* t, VirtualUser* v);
	void prepare( const char* xml);

	static ULONGLONG GetMillisecondTime();

	bool send_lgs_protocol( BasicProtocol *p);
	bool send_gts_protocol( BasicProtocol *p);

	void recv_protocol(S_UINT_16 proid, BasicProtocol *p);

	void update( ULONGLONG now);

	TestCase* get_testcase(){ return case_; }

public:
	bool is_idle(){ return wf_status_ == WFSTATUS_NONE;}
	bool is_running(){ return wf_status_ == WFSTATUS_RUNNING;}
	bool is_stop(){ return wf_status_ == WFSTATUS_STOP;}

	void start_workflow();

	void goto_next_flownode( FlowNode* nextfn);

	FlowNode* get_flownode_byid( int id);

protected:
	void build_testcase( const char* xml);

protected:
	WF_STATUS		wf_status_;

	VirtualUser*	vuser_;
	TestCase*		case_;

	std::map<int,FlowNode*>		flownodes_;
	FlowNode*		start_node_;
	FlowNode*		cur_node_;

	DaemonNode		daemon_ctrl_;
};

#endif	//__TESTCASEWORKFLOW_H__
