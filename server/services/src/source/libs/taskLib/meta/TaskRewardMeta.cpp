#include <taskLib/meta/TaskRewardMeta.h>

TaskRewardMeta* TaskRewardMeta::build_from_xml(tinyxml2::XMLElement* e)
{
	TaskRewardMeta* meta = new TaskRewardMeta();
	if (e == 0)
		return meta;
	for (tinyxml2::XMLElement* v = e->FirstChildElement("reward"); v != 0; v = v->NextSiblingElement("reward"))
	{
		PRO::TASKREWARD_TYPE t = (PRO::TASKREWARD_TYPE)XmlUtil::GetXmlAttrInt(v, "type");
		S_INT_32 itemiid = XmlUtil::GetXmlAttrInt(v, "item");
		S_INT_32 nums = XmlUtil::GetXmlAttrInt(v, "nums");

		TaskRewardMeta::TaskRewardItem it(t, itemiid, nums);
		meta->items.push_back( it);
	}

	return meta;
}

TaskRewardMeta::TaskRewardItem::TaskRewardItem(PRO::TASKREWARD_TYPE t, S_INT_32 item, S_INT_32 num)
{
	this->type = t;
	this->item_iid = item;
	this->nums = num;
}
