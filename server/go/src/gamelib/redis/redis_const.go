package redis

//----------------------------分布式任务 前缀-------------------------------
const GLOBAL_DTASK_PREFIX = "-DTASK:%s"

const GLOBAL_DTASK_RES_SVRMAINTNCE = "res-maintnce"

//-----------------------服务注册信息-------------------------------------
//服务分类信息 hash结构
const SERVICE_DETAIL_INFO = "_SERVICE:%d"

//gate扩展项
//存放gate最后一次同步的sequence
const SERVICE_GATEEXT_SLOTSEQ = "ext:slotseq"

//用户userslot资源分配结构
const HOMEGATE_WATERSLOT = "_HOMEGATE:waterslot"
const HOMEGATE_FLEXSLOT = "_HOMEGATE:flexslot"

//每组gate+home的用户资源
//homeid#gateid
//资源分配中的userslot资源列表 zset结构 member: proxytoken score: timstamp
const HOMEGATE_USERSLOT_AUTH = "_SLOT:%s:auth"

//总的可用空闲slot
const GLOBAL_USERSLOT_FREE = "_SLOT:total:free"

//-----------------------------------------------
//hash dataset
const USERLOGIN_ONLINE_INFO = "_ONLINE:%d"
const USERLOGIN_ONLINE_TOKEN = "token"
const USERLOGIN_ONLINE_LASTUPDATE = "lastup"
