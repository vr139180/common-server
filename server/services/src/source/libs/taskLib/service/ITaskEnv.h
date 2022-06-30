#ifndef __ITASKENV_H__
#define __ITASKENV_H__

#include <cmsLib/prolib/core_type.h>
#include <taskLib/task_const.h>

/************************************************************************/
/* �������ⲿ��������Դ�ӿ�                                                                     */
/************************************************************************/
//ȫ�����ݻ���
//����Ľӿں���
class IGlobalDataEnv
{
public:
};

//eTriggerType::TT_ROLEINFO_CHANGE
//xml������֧�ֵĺ�������
//xmlֻ֧��ʵʱ��get���͵������ж�
//��������Ҫ�洢��ʱ���ݵ��Ը��ӵ���lua�ű�ʵ��
typedef int(*tsxml_check_count_p0)(void);
typedef int(*tsxml_check_count_p1)(S_INT_64 id);

//�û����ݻ���
//��Ҫ�û�ģ��ʵ�ָýṹ�����֧��lua��������Ҫ�󶨶�Ӧ�ĺ�����lua
class IUserDataEnv
{
public:
	//eTriggerType::TT_ROLEINFO_CHANGE
	virtual int get_role_level() = 0;

	//eTriggerType::TT_BAG_CHANGE
	virtual int get_bag_itemnum(S_INT_64 itemiid) = 0;

public:
	//lua�����е����� u_roleiid
	virtual std::string get_luaojb_name() = 0;

	
};

#endif //__ITASKENV_H__
