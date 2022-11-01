#include <cmsLib/ServerAppWrapper.h>

#include "DataRouterApp.h"

#include <cmsLib/GlobalSettings.h>

#include <cmsLib/wincrash/crashhandler.h>
#include <cmsLib/cmsLib.h>

#include <cmsLib/net/NetHashingWithVNode.h>

#ifdef EW_PLATFORM_WINDOWS
	int wmain( s32 argc, char* argv[] )
#else
	int main( s32 argc, char* argv[] )
#endif
	{

#if EW_PLATFORM_WINDOWS
		SetUnhandledExceptionFilter(HandleCrash);
#endif

		NetHashingWithVNode<int,1000>::test();
		NetHashingWithVNode<int,2000>::test();

		svrApp.SetCmdLine( argc, argv );

		ServerAppWrapper app( &svrApp);
		app.run();

		return 0;
	}
