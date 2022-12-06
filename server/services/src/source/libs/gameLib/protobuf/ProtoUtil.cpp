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

#include <gameLib/protobuf/ProtoUtil.h>

#include <gameLib/protobuf/Proto_all.h>

using namespace google::protobuf;


bool ProtoUtil::get_location_from_msg(BasicProtocol* msg, GLoc3D& loc)
{
	const FieldDescriptor* pfield = msg->GetDescriptor()->FindFieldByLowercaseName("role_pos");
	if (pfield == 0 || pfield->type() != FieldDescriptor::TYPE_MESSAGE || pfield->is_optional() == false)
		return false;
	const Descriptor *pu = pfield->message_type();
	const Descriptor * pus = PRO::Location3D::GetDescriptor();
	if (pu != pus)
		return false;
	const Reflection* pref = msg->GetReflection();
	if (pref == 0)
		return false;

	Message* umsg = pref->MutableMessage(msg, pfield);
	if (umsg == 0)
		return false;

	PRO::Location3D* pt = dynamic_cast<PRO::Location3D*>(umsg);
	if (pt == 0)
		return false;

	loc.set_x(pt->x());
	loc.set_y(pt->y());
	loc.set_z(pt->z());

	return true;
}

bool ProtoUtil::set_location_to_msg(BasicProtocol* msg, const GLoc3D& loc)
{
	const FieldDescriptor* pfield = msg->GetDescriptor()->FindFieldByLowercaseName("role_pos");
	if (pfield == 0 || pfield->type() != FieldDescriptor::TYPE_MESSAGE || pfield->is_optional() == false)
		return false;
	const Descriptor *pu = pfield->message_type();
	const Descriptor * pus = PRO::Location3D::GetDescriptor();
	if (pu != pus)
		return false;
	const Reflection* pref = msg->GetReflection();
	if (pref == 0)
		return false;

	Message* umsg = pref->MutableMessage(msg, pfield);
	if (umsg == 0)
		return false;

	PRO::Location3D* pt = dynamic_cast<PRO::Location3D*>(umsg);
	if (pt == 0)
		return false;

	GLoc3D::copy_to(loc, pt);

	return true;
}
