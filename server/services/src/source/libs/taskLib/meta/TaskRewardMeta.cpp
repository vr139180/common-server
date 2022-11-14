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
