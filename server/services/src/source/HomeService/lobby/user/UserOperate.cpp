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
