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
#ifndef __CMDLIB_H__
#define __CMDLIB_H__

#define VERSION_DAILY
//#define VERSION_PRESSURE
//#define VERSION_RELEASE

//#define VERIFICATION_DB

#define CHEATCODE_SUPPORT
#define RECORDLOG_SUPPORT

#define PRESSURE_ANALYSIS

#ifdef VERSION_DAILY

#undef VERSION_PRESSURE
#undef VERSION_RELEASE

#undef VERIFICATION_DB

#define VERIFICATION_DB

#endif

#ifdef VERSION_PRESSURE

#undef VERSION_RELEASE

#undef VERIFICATION_DB

#undef RECORDLOG_SUPPORT

#define VERIFICATION_DB

#endif

#ifdef VERSION_RELEASE

#undef CHEATCODE_SUPPORT

#undef VERIFICATION_DB

#undef RECORDLOG_SUPPORT

#undef PRESSURE_ANALYSIS

#define VERIFICATION_DB

#endif

#endif	//__CMDLIB_H__
