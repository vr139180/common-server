#ifndef __TASKCPPOBJECTIVE_H__
#define __TASKCPPOBJECTIVE_H__

#include <vector>
#include <string>
#include <boost/unordered_map.hpp>
#include <cmsLib/core_type.h>
#include <cmsLib/util/XmlUtil.h>

#include <taskLib/task_const.h>

class CPPObjectiveParams
{
protected:
	class CPPParamValue
	{
	public:
		CPPParamValue(const char* v);
		CPPParamValue();

		int intval();
		S_INT_64 int64val();
		const char* strval();

		void setval(const char* v);

	private:
		std::string val;
	};

protected:
	CPPObjectiveParams();

public:
	static CPPObjectiveParams* build_cppparams(tinyxml2::XMLElement* e);
	static CPPObjectiveParams* build_empty() { return new CPPObjectiveParams(); }

	//index 1~≤Œ ˝Œª÷√
	bool intval_param(const char* key, int& val);
	bool int64val_param(const char* key, S_INT_64& val);
	bool strval_param(const char* key, std::string& val);

	int size() { return (int)params_.size(); }

protected:
	CPPParamValue* get_param( const char* key);

private:
	boost::unordered_map<std::string, CPPParamValue> params_;
};

#endif //__TASKCPPOBJECTIVE_H__
