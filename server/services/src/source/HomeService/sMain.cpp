#include <cmsLib/ServerAppWrapper.h>

#include "HomeServiceApp.h"

#include <cmsLib/GlobalSettings.h>

#include <cmsLib/wincrash/crashhandler.h>
#include <cmsLib/cmsLib.h>

#ifdef EW_PLATFORM_WINDOWS
	int wmain( s32 argc, char* argv[] )
#else
	int main( s32 argc, char* argv[] )
#endif
	{
#if EW_PLATFORM_WINDOWS
		SetUnhandledExceptionFilter(HandleCrash);
#endif

		svrApp.SetCmdLine( argc, argv );

		ServerAppWrapper app( &svrApp);
		app.run();

		return 0;
	}
