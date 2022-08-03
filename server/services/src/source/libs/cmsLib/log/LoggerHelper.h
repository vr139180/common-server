#ifndef __LOGGERHELPER__HEADER__H__
#define __LOGGERHELPER__HEADER__H__

class LoggerHelper
{
private:
	LoggerHelper();
	~LoggerHelper();

public:
	static void InitLogger( const char* file);
	static void Shutdown();
	static void Debug( const char* model, const char* msg,...);
	static void Info( const char* model, const char* msg,...);
	static void Warn( const char* model, const char* msg,...);
	static void Error( const char* model,const  char* msg,...);
	static void Fatal( const char* model, const char* msg,...);

private:
	static void regist_custom_sink();
};

#endif //__LOGGERHELPER__HEADER__H__
