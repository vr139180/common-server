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

#ifndef __WORLDUTIL_H__
#define __WORLDUTIL_H__

#include <cmsLib/core_type.h>

class WorldUtil
{
private:
	WorldUtil() {}

public:
	//����gameid
	static S_INT_64 build_gameid(S_INT_64 serviceid, S_INT_32 channel, S_INT_32 regionid);
	static S_INT_64 get_svrid_from_gameid(S_INT_64 gameid);
	static S_INT_32 get_channel_from_gameid(S_INT_64 gameid);
	static S_INT_32 get_regionid_from_gameid(S_INT_64 gameid);

};

#endif //__WORLDUTIL_H__
