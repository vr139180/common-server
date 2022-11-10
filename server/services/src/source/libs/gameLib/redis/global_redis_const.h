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

//------------------------sEureka cluster���------------------------------
//���eureka master�ڵ���Ϣ, hash
REDIS_KEYDEFINED(EUREKA_MASTER_NODE, "_EUREKA:MASTER");
REDIS_KEYDEFINED(FIELD_MASTER_NODE_IID, "iid");
REDIS_KEYDEFINED(FIELD_MASTER_NODE_SVR, "svrinfo");

//master�ڵ�ʧЧʱ�� 5 seconds
#define EUREKA_MASTER_NODE_TIMEOUT	5*1000

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



//�û����ݶ���

REDISKEY_GLOBAL_NS_END

#endif //__GLOBAL_REDIS_CONST_H__