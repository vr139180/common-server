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

#ifndef __DTASKREDIS_H__
#define __DTASKREDIS_H__

#include <string>
#include <cmsLib/core_type.h>
#include <cmsLib/redis/RedisClient.h>

class DTaskRedis : public IRedisSerializer
{
public:
	DTaskRedis();
	DTaskRedis(const char* name, S_INT_64 serviceiid, S_INT_32 step);

	//�Ƿ���ѭ������
	bool is_looptask() { return task_step > 0; }

	/**
	* �´�����������λ����
	* @param randscope �´��������ĸ���������䣬������ϵͳ�������ֲ��������
	* @return 0:û����һ��ʱ�� >0 ��ʱ�����,��λ����
	**/
	S_INT_32 next_task_timer( S_INT_32 randscope);

	S_INT_32 get_task_step() { return task_step; }
	std::string get_task_name() { return task_name; }
	S_INT_64 get_service_iid() { return service_iid; }

protected:
	//------------------implementation IRedisSerializer---------------------
	virtual bool to_json(std::string& val);
	virtual bool from_json(boost::json::value& root);

private:
	//����iid
	S_INT_64	service_iid;
	//task name
	std::string task_name;
	//��ʼʱ��
	S_INT_64	start_time;
	//������,��Ϊ��λ. step==0 ��ʾһ��������
	S_INT_32	task_step;
};

#endif //__DTASKREDIS_H__