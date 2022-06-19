#ifndef __GLOBALOPTION_H__
#define __GLOBALOPTION_H__

#include <string>

namespace config {
	typedef struct tag_GlobalOption
	{
		//整个拓扑的服务数量
		int svrnum_min;

		//eureka首届点
		std::string eip;
		int eport;
	}GlobalOption;
}

#endif	//__GLOBALOPTION_H__
