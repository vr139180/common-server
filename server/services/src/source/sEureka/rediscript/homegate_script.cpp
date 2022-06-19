#include "rediscript/homegate_script.h"

#include <gameLib/global_const.h>

/*
keys: HOMEGATE_GATE_WAIT,HOMEGATE_HOME_WAIT,HOMEGATE_AUTHING，SERVICE_MAINTNCE H
vals: gateid, homeid, bindtoken，0
rets: [0]:0 succ, other failed [1]:if succ, save homeiid others 0
1:gateiid不存在 2:homeid无效
*/
std::string HomeGateConst::gatebindhome_request = "";

/*
keys: HOMEGATE_GATE_WAIT,HOMEGATE_HOME_WAIT,HOMEGATE_AUTHING,
	  HOMEGATE_MIX,HOMEGATE_WATERSLOT,HOMEGATE_FLEXSLOT,
	  GLOBAL_USERSLOT_FREE,
	  SERVICE_MAINTNCE H, SERVICE_MAINTNCE G
vals: gateid, homeid, bindtoken,
	  GATEHOME_GROUP_NUM, MIN_GATEHOME_GROUP_NUM, 0,
	  0, 0, 0
rets: [0]:0 succ, other failed
1:homeid+gateiid不存在HOMEGATE_AUTHING 2:bindtoken不存在
*/
std::string HomeGateConst::gatebindhome_confirm = "";

/*
keys: HOMEGATE_GATE_WAIT,HOMEGATE_HOME_WAIT,GLOBAL_USERSLOT_FREE,
	  HOMEGATE_MIX,HOMEGATE_WATERSLOT,HOMEGATE_FLEXSLOT,
vals: sid, type(1:gate 2:home), 0,
	  GATEHOME_GROUP_NUM, MIN_GATEHOME_GROUP_NUM, 0,
rets: [0]:0 succ, other failed
1:homeid+gateiid不存在HOMEGATE_AUTHING 2:bindtoken不存在
*/
std::string HomeGateConst::gatebindhome_maintnce = "";

/*
keys: HOMEGATE_GATE_WAIT,HOMEGATE_HOME_WAIT,HOMEGATE_AUTHING,
	  SERVICE_MAINTNCE H, SERVICE_MAINTNCE G
vals: gateid,homeid,0
	  0,0
rets: [0]:0 succ, other failed
1:homeid+gateiid不存在HOMEGATE_AUTHING 2:bindtoken不存在
*/
std::string HomeGateConst::gatebindhome_authtimeout = "";