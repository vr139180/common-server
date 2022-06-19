#ifndef __HOMEGATE_SCRIPT_H__
#define __HOMEGATE_SCRIPT_H__

#include <string>

class HomeGateConst
{
private:
	HomeGateConst() {}
public:
	static std::string gatebindhome_request;
	static std::string gatebindhome_confirm;
	static std::string gatebindhome_maintnce;
	static std::string gatebindhome_authtimeout;
};

#endif //__HOMEGATE_SCRIPT_H__