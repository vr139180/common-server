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

#include "gameLib/commons/GLoc3D.h"

#include <cmsLib/util/ShareUtil.h>

const GLoc3D& GLoc3D::zero_point()
{
	static GLoc3D zero(0.0, 0.0, 0.0);
	return zero;
}

void GLoc3D::copy_to(const GLoc3D& loc, PRO::Location3D* pos)
{
	pos->set_x(loc.x());
	pos->set_y(loc.y());
	pos->set_z(loc.z());
}

void GLoc3D::copy_to(PRO::Location3D* pos, GLoc3D& loc)
{
	loc.set_x(pos->x());
	loc.set_y(pos->y());
	loc.set_z(pos->z());
}

GLoc3D& GLoc3D::operator = (const GLoc3D& v)
{
	this->x_ = v.x();
	this->y_ = v.y();
	this->z_ = v.z();

	return *this;
}

//{000,000}
bool GLoc3D::build_from_str(std::string& str, GLoc3D& p)
{
	std::size_t const m1 = str.find_first_of("{");
	std::size_t const m2 = str.find_last_of("}");
	if (m1 == std::string::npos || m2 == std::string::npos)
		return false;
	std::string str2 = str.substr(m1 + 1, m2 - m1 - 1);
	std::vector<float> dat;
	if (!ShareUtil::splitstr2float(str2.c_str(), ",", dat) || dat.size() != 3)
		return false;

	p.set_x(dat[0]);
	p.set_y(dat[1]);
	p.set_z(dat[2]);

	return true;
}

std::string GLoc3D::to_string() const
{
	return ShareUtil::str_format<64>("{%.3f,%.3f,%.3f}", x_, y_, z_);
}

bool GLoc3D::is_zero_point() const
{
	if ((int)(x_ * 1000) == 0 && (int)(y_ * 1000) == 0 && (int)(z_ * 1000) == 0)
		return true;
	return false;
}
