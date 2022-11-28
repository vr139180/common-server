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

#include "worldsLib/utils/WorldUtil.h"

//50:service 14:channel
#define GAMEID_MASK_ZERO_H	0b0000000000000000000000000000000000000000000000000011111111111111
#define GAMEID_MASK_ZERO_L	0b1111111111111111111111111111111111111111111111111100000000000000

#define MAKE_GAMEID( SVRID, CID, GAMEID) { GAMEID = ((SVRID<<14)&GAMEID_MASK_ZERO_L)|CID;}
#define PARSE_GAMEID( GAMEID, SVRID, CID) { SVRID = (int)((GAMEID&GAMEID_MASK_ZERO_L)>>14); CID = (int)(GAMEID&GAMEID_MASK_ZERO_H);}
#define PARSE_GAMEID_SERVICEID( GAMEID, SVRID) { SVRID = (int)((GAMEID&GAMEID_MASK_ZERO_L)>>14);}
#define PARSE_GAMEID_CID( GAMEID, CID) { CID = (int)(GAMEID&GAMEID_MASK_ZERO_H);}


S_INT_64 WorldUtil::build_gameid(S_INT_64 serviceid, S_INT_32 channel)
{
	S_INT_64 gid = 0;
	MAKE_GAMEID(serviceid, channel, gid);
	return gid;
}
