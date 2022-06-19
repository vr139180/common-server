#ifndef __SERVERAPPWRAPPER_H__
#define __SERVERAPPWRAPPER_H__

#include <cmsLib/ServerAppBase.h>

class ServerAppWrapper
{
public:
	ServerAppWrapper( ServerAppBase* app);

	void run();

protected:

	bool init_app();
	void uninit_app();

	static void signal_handler( s32 signal);

private:
	static ServerAppBase*	app_;
};

#endif // __SERVERAPPWRAPPER_H__
