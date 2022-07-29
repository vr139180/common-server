#ifndef __LOG_H__
#define __LOG_H__

#include <cmsLib/cmsLib.h>

#include <cmsLib/log/LoggerHelper.h>

#define logInit(x)	LoggerHelper::InitLogger(x);
#define logShutdown	LoggerHelper::Shutdown();

#define logDebug( cat, fmt, ...)	LoggerHelper::Debug( cat, fmt, ##__VA_ARGS__)
#define logInfo( cat, fmt, ...)		LoggerHelper::Info( cat, fmt, ##__VA_ARGS__)
#define logWarn( cat, fmt, ...)		LoggerHelper::Warn( cat, fmt, ##__VA_ARGS__)
#define logError( cat, fmt, ...)	LoggerHelper::Error( cat, fmt, ##__VA_ARGS__)
#define logFatal( cat, fmt, ...)	LoggerHelper::Fatal( cat, fmt, ##__VA_ARGS__)

//log category
#define out_runtime		"rt"	//runtime
#define out_gamedata	"gd"	//game data
#define out_monitor		"mt"	//service monitor

#endif // __LOG_H__
