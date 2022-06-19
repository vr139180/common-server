#ifndef __CONDITIONCONTEXT__H__
#define __CONDITIONCONTEXT__H__

#include "cmsLib/condition/IPropertyOperator.h"

#include <assert.h>
#include <map>
#include <string>

/**
* @class ConditionContext
* 
* @brief �����ж��õ��Ļ�����
**/
class ConditionContext
{
public:
	typedef std::map< std::string, IPropertyOperator*>	CONTEXT_MAP;

public:
	ConditionContext(){}

	void reset(){ context_.clear();}

	void regist_context( const char* objname, IPropertyOperator* obj){
		context_[objname] =obj;
	}

	IPropertyOperator* get_object( const char* objname){
		CONTEXT_MAP::iterator fiter =context_.find( objname);
		if( fiter == context_.end())
			return 0;
		return fiter->second;
	}

protected:
	//֧�ֵĶ����б� obj.property
	CONTEXT_MAP	context_;
};

#endif	//__CONDITIONCONTEXT__H__
