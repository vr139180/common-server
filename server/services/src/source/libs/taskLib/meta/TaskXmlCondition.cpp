#include <taskLib/meta/TaskXmlCondition.h>

#include <gameLib/LogExt.h>

TaskXmlCondition::TaskXmlCondition():is_leaf_( false)
{
}

TaskXmlCondition::~TaskXmlCondition()
{
	release();
}

void TaskXmlCondition::release()
{
	for (int ii = 0; ii < childen_conds_.size(); ++ii)
		delete childen_conds_[ii];
	childen_conds_.clear();

	for (int ii = 0; ii < objectives_.size(); ++ii)
		delete objectives_[ii];
	objectives_.clear();
}

TaskXmlCondition* TaskXmlCondition::build_taskcondition(tinyxml2::XMLElement* e, TaskXmlCondition* p)
{
	TaskXmlCondition* txc = new TaskXmlCondition();
	txc->parent_ = p;
	std::unique_ptr< TaskXmlCondition> ptr(txc);

	std::string rl = XmlUtil::GetXmlAttrStr(e, "relation");
	if (rl == "and")
		txc->node_relation_ = CondRelationType::CondRelationType_And;
	else if (rl == "or")
		txc->node_relation_ = CondRelationType::CondRelationType_Or;
	else if (rl == "not")
		txc->node_relation_ = CondRelationType::CondRelationType_Not;
	else
	{
		logError(out_sys, "load condition failed....");
		return 0;
	}

	//children
	for (tinyxml2::XMLElement* cc = e->FirstChildElement("cond"); cc != 0; cc = cc->NextSiblingElement("cond"))
	{
		TaskXmlCondition* pchild = TaskXmlCondition::build_taskcondition(cc, txc);
		if (pchild == 0)
		{
			logError(out_sys, "load condition failed....");
			return 0;
		}

		txc->childen_conds_.push_back(pchild);
	}

	for (tinyxml2::XMLElement* cc = e->FirstChildElement("objective"); cc != 0; cc = cc->NextSiblingElement("objective"))
	{
		XmlObjectvie* xobj = XmlObjectvie::build_objective(cc);
		if (xobj == 0)
		{
			logError(out_sys, "load xmlobject failed....");
			return 0;
		}
		
		txc->objectives_.push_back(xobj);
	}

	if (txc->childen_conds_.size() > 0 && txc->objectives_.size() > 0)
	{
		logError(out_sys, "cond child node > 0 and objective node > 0, load condition failed....");
		return 0;
	}

	txc->is_leaf_ = (txc->childen_conds_.size() == 0);

	return ptr.release();
}

bool TaskXmlCondition::check_results(std::vector<bool>& rets)
{
	if (node_relation_ == CondRelationType::CondRelationType_And)
	{
		for (size_t ii = 0; ii < rets.size(); ++ii)
		{
			if (!rets[ii])
				return false;
		}

		return true;
	}
	else if (node_relation_ == CondRelationType::CondRelationType_Or)
	{
		for (size_t ii = 0; ii < rets.size(); ++ii)
		{
			if (rets[ii])
				return true;
		}

		return false;
	}
	else if (node_relation_ == CondRelationType::CondRelationType_Not)
	{
		//first all and, then not
		bool br = true;
		for (size_t ii = 0; ii < rets.size(); ++ii)
		{
			if (!rets[ii])
			{
				br = false;
				break;
			}
		}

		return !br;
	}

	return false;
}