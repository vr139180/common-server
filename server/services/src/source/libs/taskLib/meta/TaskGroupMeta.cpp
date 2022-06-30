#include <taskLib/meta/TaskGroupMeta.h>

#include <gameLib/config/ConfigTool.h>

TaskGroupMeta::~TaskGroupMeta()
{
	release();
}

void TaskGroupMeta::release()
{
	for (CELLS_MAP::iterator iter = cells_.begin(); iter != cells_.end(); ++iter)
	{
		delete iter->second;
	}
	cells_.clear();
}

TaskGroupMeta* TaskGroupMeta::load_taskgroup(const char* filename)
{
	TaskGroupMeta *pmeta = new TaskGroupMeta();
	std::unique_ptr<TaskGroupMeta> ptr( pmeta);

	std::string str = ConfigTool::get_instance().get_txtfilecontent( filename, true);
	tinyxml2::XMLDocument doc;
	if (!XmlUtil::loadfromstring(str.c_str(), doc))
		return 0;

	if (!pmeta->load_from_xml(doc.RootElement()))
		return 0;

	return ptr.release();
}

bool TaskGroupMeta::load_from_xml( tinyxml2::XMLElement* e)
{
	this->group_iid_ = XmlUtil::GetXmlAttrInt(e, "iid", -1);
	start_celliid_ = XmlUtil::GetXmlAttrInt(e, "sc", -1);
	end_celliid_ = XmlUtil::GetXmlAttrInt(e, "ec", -1);
	last_cell_iid_ = XmlUtil::GetXmlAttrInt(e, "last_celliid", -1);
	version_ = XmlUtil::GetXmlAttrStr(e, "version");

	tinyxml2::XMLElement* ce = e->FirstChildElement("cell");
	if (ce == 0)
		return false;

	for (; ce != 0; ce = ce->NextSiblingElement("cell"))
	{
		TaskGroupCellMeta* cell = TaskGroupCellMeta::load_groupcell(ce, this);
		if (cell == 0)
			return false;

		cells_[cell->get_iid()] = cell;
	}

	return true;
}
