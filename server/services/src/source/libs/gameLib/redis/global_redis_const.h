#ifndef __GLOBAL_REDIS_CONST_H__
#define __GLOBAL_REDIS_CONST_H__

#include <string>

#define REDISKEY_GLOBAL_NS_BEGIN namespace rdkey { namespace global {
#define REDISKEY_GLOBAL_NS_END } }

#define USED_REDISKEY_GLOBAL_NS using namespace rdkey::global;

#define REDIS_KEYDEFINED( KEY, VAL) static const char* KEY = VAL

REDISKEY_GLOBAL_NS_BEGIN

//----------------------------ȫ������id-----------------------------------
REDIS_KEYDEFINED( GLOBAL_IDGEN_HKEY, "-GLOBAL:IDGEN-");
REDIS_KEYDEFINED( GLOBAL_IDGEN_SERVICE_F, "_svr:iid");
REDIS_KEYDEFINED( GLOBAL_IDGEN_DBID, "_db_id:%s");

//----------------------------�ֲ�ʽ���� ǰ׺-------------------------------
REDIS_KEYDEFINED(GLOBAL_DTASK_PREFIX, "-DTASK:%s");

//eureka ״̬ά������
REDIS_KEYDEFINED(GLOBAL_DTASK_EUREKAMAINTNCE, "eureka-maintnce");

//����״̬ά��
REDIS_KEYDEFINED(GLOBAL_DTASK_SVRMAINTNCE_ALL, "svr-maintnce-all");
REDIS_KEYDEFINED(GLOBAL_DTASK_SVRMAINTNCE_RES, "svr-maintnce-res");
REDIS_KEYDEFINED(GLOBAL_DTASK_SVRMAINTNCE_HOME, "svr-maintnce-home");
REDIS_KEYDEFINED(GLOBAL_DTASK_SVRMAINTNCE_GATE, "svr-maintnce-gate");
REDIS_KEYDEFINED(GLOBAL_DTASK_SVRMAINTNCE_LOGIN, "svr-maintnce-login");
REDIS_KEYDEFINED(GLOBAL_DTASK_SVRMAINTNCE_GATEHOMEAUTH, "svr-maintnce-gatehomeauth");

//------------------------sEureka cluster���------------------------------
REDIS_KEYDEFINED(EUREKA_CLUSTER_ALLS, "_EUREKA:ALLS");
//���eureka�ķ��������Ϣ zset�ṹ
REDIS_KEYDEFINED(EUREKA_CLUSTER_QUEUE, "_EUREKA:QUEUE");
//��Ű󶨵ķ�����Ϣ
REDIS_KEYDEFINED(EUREKA_CLUSTER_SVRBIND, "_EUREKA:%ld:BIND");

//eureka�ڵ�ÿ 3s����һ��lastupdate, 6s����queue
#define TIMER_EUREKA_UPDATELASTIME_STEP		3000
#define TIMER_EUREKA_QUEUE_STEP				6000


//-----------------------����ע����Ϣ-------------------------------------
//���������Ϣ hash�ṹ
REDIS_KEYDEFINED(SERVICE_CATEGORY, "_SVRS:%s");
//������ϸ��Ϣ hash�ṹ
REDIS_KEYDEFINED(SERVICE_DETAIL_INFO, "_SERVICE:%ld");
REDIS_KEYDEFINED(SERVICE_DETAIL_SVRTYPE, "base:type");
REDIS_KEYDEFINED(SERVICE_DETAIL_TOKEN, "base:token");
REDIS_KEYDEFINED(SERVICE_DETAIL_STATUS, "base:status");
REDIS_KEYDEFINED(SERVICE_DETAIL_SVRINFO, "base:sinfo");
//gate��չ��
//���gate���һ��ͬ����sequence
REDIS_KEYDEFINED(SERVICE_GATEEXT_SLOTSEQ, "ext:slotseq");

//home��չ��


//������Ч��ά��
REDIS_KEYDEFINED(SERVICE_MAINTNCE, "_SVRS:%s:QUEUE");

//����ע�ᶨʱ
#define TIMER_SERVICE_UPDATSYNC_STEP		4000
//service�ڵ�ÿ 3s����һ��lastupdate, 6s����queue
#define TIMER_SERVICE_QUEUE_STEP				6000

//---------------------Home+Gateƥ��������--------------------------------
//����Ե�home���� set�ṹ
REDIS_KEYDEFINED(HOMEGATE_HOME_WAIT, "_HOMEGATE:wait:home");
//����Ե�gate���� set�ṹ
REDIS_KEYDEFINED(HOMEGATE_GATE_WAIT, "_HOMEGATE:wait:gate");
//����Ե�gate���� set�ṹ
REDIS_KEYDEFINED(HOMEGATE_MIX, "_HOMEGATE:mix");
//����е� hset�ṹ member: homeid#gateid, score: ��Կ�ʼ��ʱ���
REDIS_KEYDEFINED(HOMEGATE_AUTHING, "_HOMEGATE:auth");

//�û�userslot��Դ����ṹ
//ϵͳ֧�ֵ��������� hset�ṹ member:gateid#lobbyid score:���õ�slot����
REDIS_KEYDEFINED(HOMEGATE_WATERSLOT, "_HOMEGATE:waterslot");
//ϵͳ��չ����Դ hset�ṹ member:gateid#homeid score:���õ�slot����
REDIS_KEYDEFINED(HOMEGATE_FLEXSLOT, "_HOMEGATE:flexslot");

//ÿ��gate+home���û���Դ
//homeid#gateid
//��Դ�����е�userslot��Դ�б� zset�ṹ member: proxytoken score: timstamp
REDIS_KEYDEFINED(HOMEGATE_USERSLOT_AUTH, "_SLOT:%ld#%ld:auth");

//�ܵĿ��ÿ���slot
REDIS_KEYDEFINED(GLOBAL_USERSLOT_FREE, "_SLOT:total:free");

//�û����ݶ���

REDISKEY_GLOBAL_NS_END

#endif //__GLOBAL_REDIS_CONST_H__