#ifndef __TASKXMLOBJECTIVE_H__
#define __TASKXMLOBJECTIVE_H__

#include <vector>
#include <string>
#include <boost/unordered_map.hpp>
#include <cmsLib/prolib/core_type.h>
#include <cmsLib/util/XmlUtil.h>

#include <taskLib/task_const.h>

class XmlObjectiveParams
{
protected:
	class XmlParamValue
	{
	public:
		XmlParamValue(const char* v);
		XmlParamValue();

		int intval();
		S_INT_64 int64val();
		const char* strval();

		void setval(const char* v);

	private:
		std::string val;
	};

protected:
	XmlObjectiveParams();

public:
	static XmlObjectiveParams* build_xmlparams(const char* str);

	//index 1~����λ��
	bool intval_param(int index, int& val);
	bool int64val_param(int index, S_INT_64& val);
	bool strval_param(int index, std::string& val);

	int size() { return params_.size(); }

protected:
	XmlParamValue* get_param(int index);

private:
	std::vector<XmlParamValue>	params_;
};

typedef enum tag_XmlObjectiveOperType {
	XmlObjectiveOperType_Great = 1,
	XmlObjectiveOperType_GreatE,
	XmlObjectiveOperType_Less,
	XmlObjectiveOperType_LessE,
	XmlObjectiveOperType_Equal,
	XmlObjectiveOperType_NotEqual,
	XmlObjectiveOperType_Between,
}XmlObjectiveOperType;

class XmlObjectvie
{
protected:
	XmlObjectvie();

	void release();

public:
	static XmlObjectvie* build_objective(tinyxml2::XMLElement* e);
	virtual ~XmlObjectvie();

	const std::string& get_funname() { return fun_name_; }
	XmlObjectiveParams* get_params() { return fun_params_; }

	bool check_result(S_INT_64 retval);

private:
	eTriggerType			trigger_;
	std::string				fun_name_;
	XmlObjectiveParams*		fun_params_;
	XmlObjectiveOperType	oper_;
	XmlObjectiveParams*		oper_params_;
};

#endif //__TASKXMLOBJECTIVE_H__
