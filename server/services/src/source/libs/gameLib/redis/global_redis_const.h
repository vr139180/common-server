#ifndef __GLOBAL_REDIS_CONST_H__
#define __GLOBAL_REDIS_CONST_H__

#include <string>

#define REDISKEY_GLOBAL_NS_BEGIN namespace rdkey { namespace global {
#define REDISKEY_GLOBAL_NS_END } }

#define USED_REDISKEY_GLOBAL_NS using namespace rdkey::global;

#define REDIS_KEYDEFINED( KEY, VAL) static const char* KEY = VAL

REDISKEY_GLOBAL_NS_BEGIN

//----------------------------全局自增id-----------------------------------
REDIS_KEYDEFINED( GLOBAL_IDGEN_HKEY, "-GLOBAL:IDGEN-");
REDIS_KEYDEFINED( GLOBAL_IDGEN_SERVICE_F, "_svr:iid");
REDIS_KEYDEFINED( GLOBAL_IDGEN_DBID, "_db_id:%s");

//----------------------------分布式任务 前缀-------------------------------
REDIS_KEYDEFINED(GLOBAL_DTASK_PREFIX, "-DTASK:%s");

//eureka 状态维护任务
REDIS_KEYDEFINED(GLOBAL_DTASK_EUREKAMAINTNCE, "eureka-maintnce");

//服务状态维护
REDIS_KEYDEFINED(GLOBAL_DTASK_SVRMAINTNCE_ALL, "svr-maintnce-all");
REDIS_KEYDEFINED(GLOBAL_DTASK_SVRMAINTNCE_RES, "svr-maintnce-res");
REDIS_KEYDEFINED(GLOBAL_DTASK_SVRMAINTNCE_HOME, "svr-maintnce-home");
REDIS_KEYDEFINED(GLOBAL_DTASK_SVRMAINTNCE_GATE, "svr-maintnce-gate");
REDIS_KEYDEFINED(GLOBAL_DTASK_SVRMAINTNCE_LOGIN, "svr-maintnce-login");
REDIS_KEYDEFINED(GLOBAL_DTASK_SVRMAINTNCE_GATEHOMEAUTH, "svr-maintnce-gatehomeauth");

//------------------------sEureka cluster相关------------------------------
REDIS_KEYDEFINED(EUREKA_CLUSTER_ALLS, "_EUREKA:ALLS");
//存放eureka的服务更新信息 zset结构
REDIS_KEYDEFINED(EUREKA_CLUSTER_QUEUE, "_EUREKA:QUEUE");
//存放绑定的服务信息
REDIS_KEYDEFINED(EUREKA_CLUSTER_SVRBIND, "_EUREKA:%ld:BIND");

//eureka节点每 3s更新一个lastupdate, 6s回溯queue
#define TIMER_EUREKA_UPDATELASTIME_STEP		3000
#define TIMER_EUREKA_QUEUE_STEP				6000


//-----------------------服务注册信息-------------------------------------
//服务分类信息 hash结构
REDIS_KEYDEFINED(SERVICE_CATEGORY, "_SVRS:%s");
//服务详细信息 hash结构
REDIS_KEYDEFINED(SERVICE_DETAIL_INFO, "_SERVICE:%ld");
REDIS_KEYDEFINED(SERVICE_DETAIL_SVRTYPE, "base:type");
REDIS_KEYDEFINED(SERVICE_DETAIL_TOKEN, "base:token");
REDIS_KEYDEFINED(SERVICE_DETAIL_STATUS, "base:status");
REDIS_KEYDEFINED(SERVICE_DETAIL_SVRINFO, "base:sinfo");
//gate扩展项
//存放gate最后一次同步的sequence
REDIS_KEYDEFINED(SERVICE_GATEEXT_SLOTSEQ, "ext:slotseq");

//home扩展项


//服务有效性维护
REDIS_KEYDEFINED(SERVICE_MAINTNCE, "_SVRS:%s:QUEUE");

//服务注册定时
#define TIMER_SERVICE_UPDATSYNC_STEP		4000
//service节点每 3s更新一个lastupdate, 6s回溯queue
#define TIMER_SERVICE_QUEUE_STEP				6000

//---------------------Home+Gate匹配数据项--------------------------------
//待配对的home服务 set结构
REDIS_KEYDEFINED(HOMEGATE_HOME_WAIT, "_HOMEGATE:wait:home");
//待配对的gate服务 set结构
REDIS_KEYDEFINED(HOMEGATE_GATE_WAIT, "_HOMEGATE:wait:gate");
//已配对的gate服务 set结构
REDIS_KEYDEFINED(HOMEGATE_MIX, "_HOMEGATE:mix");
//配对中的 hset结构 member: homeid#gateid, score: 配对开始的时间戳
REDIS_KEYDEFINED(HOMEGATE_AUTHING, "_HOMEGATE:auth");

//用户userslot资源分配结构
//系统支持的最少人数 hset结构 member:gateid#lobbyid score:可用的slot数量
REDIS_KEYDEFINED(HOMEGATE_WATERSLOT, "_HOMEGATE:waterslot");
//系统扩展的资源 hset结构 member:gateid#homeid score:可用的slot数量
REDIS_KEYDEFINED(HOMEGATE_FLEXSLOT, "_HOMEGATE:flexslot");

//每组gate+home的用户资源
//homeid#gateid
//资源分配中的userslot资源列表 zset结构 member: proxytoken score: timstamp
REDIS_KEYDEFINED(HOMEGATE_USERSLOT_AUTH, "_SLOT:%ld#%ld:auth");

//总的可用空闲slot
REDIS_KEYDEFINED(GLOBAL_USERSLOT_FREE, "_SLOT:total:free");

//用户数据定义

REDISKEY_GLOBAL_NS_END

#endif //__GLOBAL_REDIS_CONST_H__