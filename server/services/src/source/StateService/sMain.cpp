// Copyright 2021 common-server Authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include <cmsLib/ServerAppWrapper.h>

#include "StateServiceApp.h"

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
