#ifndef __FLOWCONDITION_H__
#define __FLOWCONDITION_H__

#include <string>
#include <map>
#include <list>

#include <cmsLib/util/XmlUtil.h>

class FlowNode;
class VirtualUser;
class TestCaseWorkFlow;

class FlowCondition
{
public:
	FlowCondition();
	virtual ~FlowCondition();

	void build_from_xml_sub( tinyxml2::XMLElement* f);
	void link_after_build( VirtualUser* v, TestCaseWorkFlow* f);

	virtual void init_before_start();

	FlowNode* get_next_flownode(){ return next_flownode_;}

	bool is_condition_true();

protected:
	int			next_flownode_id_;
	FlowNode*	next_flownode_;

	std::string	condition_;

	VirtualUser*		vuser_;
	TestCaseWorkFlow*	wf_;
};

#endif	//__FLOWCONDITION_H__
