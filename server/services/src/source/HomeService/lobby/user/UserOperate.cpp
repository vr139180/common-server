#include "lobby/user/UserOperate.h"

#include <gameLib/protobuf/cpp/db_internal.pb.h>

using namespace google::protobuf;

void UserOperate::adjust_dels(S_INT_64 dbver)
{
	if (get_data_dels() == 0)
		return;

	PRO::DBRowDeletes* dbdels = dynamic_cast<PRO::DBRowDeletes*>(get_data_dels());
	RepeatedPtrField<PRO::DBRowDel>* dels = dbdels->mutable_dels();
	if (dels == 0 || dels->size() == 0)
		return;

	for (RepeatedPtrField<PRO::DBRowDel>::const_iterator iter = dels->begin(); iter != dels->end(); )
	{
		const PRO::DBRowDel& item = (*iter);
		if (item.ver_() <= dbver)
		{
			iter = dels->erase(iter);
			mark_data_dels_updated();
		}
		else
		{
			++iter;
		}
	}
}
