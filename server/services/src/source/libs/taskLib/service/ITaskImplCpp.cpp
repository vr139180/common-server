#include <taskLib/service/ITaskImplCpp.h>

#include <taskLib/cpptasks/MyTaskCppTemplate.h>

//task µœ÷√˚≥∆
#define TASKCPPIMPL_TEMPLATE "template"

ITaskImplCpp* ITaskImplCpp::build_cppimpl_from_name(const char* name)
{
	std::string strname = name;
	if (strname == TASKCPPIMPL_TEMPLATE)
		return new MyTaskCppTemplate();

	return 0;
}
