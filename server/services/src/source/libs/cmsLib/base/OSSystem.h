#ifndef __OSSYSTEM_H__
#define __OSSYSTEM_H__

#include <cmsLib/GlobalSettings.h>
#include <cmsLib/OSDriver.h>

class OSSystem
{
private:
	OSSystem(){}

public:
	static bool	init();
	static void	uninit();

public:
	static OSDriver*        mOS;
	static std::string      mOSType;

	static std::string      work_path2_;
	static std::string		script_path_;
};

#endif // __OSSYSTEM_H__
