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

#include <worldsLib/geometry/common_types.h>

#include <cmsLib/util/ShareUtil.h>
#include <worldsLib/cmsworld_const.h>

//---------------------------------------------CMSPointXZ---------------------------------
const CMSPointXZ& CMSPointXZ::zero_point()
{
	static CMSPointXZ zero(0.0, 0.0);
	return zero;
}

CMSPointXZ& CMSPointXZ::operator = (const CMSPointXZ& v)
{
	this->x_ = v.x();
	this->z_ = v.z();

	return *this;
}

//{000,000}
bool CMSPointXZ::build_from_str(std::string& str, CMSPointXZ& p)
{
	std::size_t const m1 = str.find_first_of("{");
	std::size_t const m2 = str.find_last_of("}");
	if (m1 == std::string::npos || m2 == std::string::npos)
		return false;
	std::string str2 = str.substr( m1 + 1, m2 - m1 - 1);
	std::vector<float> dat;
	if (!ShareUtil::splitstr2float(str2.c_str(), ",", dat) || dat.size() != 2)
		return false;

	p.set_x(dat[0]);
	p.set_z(dat[1]);

	return true;
}

std::string CMSPointXZ::to_string()
{
	return ShareUtil::str_format<128>("{%10.3f,%10.3f}", x_, z_);
}

//---------------------------------------------CMSBox---------------------------------
CMSBox::CMSBox():left_top_(CMSPointXZ::zero_point()), width_(0.0), height_(0.0)
{
}

CMSBox::CMSBox(float x, float z, float w, float h):left_top_(x, z), width_( w), height_( h)
{
}

void CMSBox::init_box(const CMSPointXZ& pos, float w, float h)
{
	left_top_ = pos;
	width_ = w;
	height_ = h;
}

bool CMSBox::is_point_in_box(const CMSPointXZ& p)
{
	float dx = p.x() - left_top_.x();
	float dz = left_top_.z() - p.z();
	return (dx >= 0 && dx < width_) && (dz > 0 && dz <= height_);
}

bool CMSBox::is_point_in_box(const GLoc3D& loc)
{
	float dx = loc.x() - left_top_.x();
	float dz = left_top_.z() - loc.z();
	return (dx >= 0 && dx < width_) && (dz > 0 && dz <= height_);
}

CMSBox& CMSBox::operator = (const CMSBox& v)
{
	this->left_top_ = v.left_top_;
	this->width_ = v.width_;
	this->height_ = v.height_;

	return *this;
}

void CMSBox::extend_left(int cells)
{
	float ps = REGION_CELL_SIZE * cells * 1.0;
	float x = left_top_.x() - ps;
	left_top_.set_x(x);
	this->width_ += ps;
}

void CMSBox::extend_right(int cells)
{
	float ps = REGION_CELL_SIZE * cells * 1.0;
	this->width_ += ps;
}

void CMSBox::extend_top(int cells)
{
	float ps = REGION_CELL_SIZE * cells * 1.0;
	float z = left_top_.z() + ps;
	left_top_.set_z(z);
	this->height_ += ps;
}

void CMSBox::extend_buttom(int cells)
{
	float ps = REGION_CELL_SIZE * cells * 1.0;
	this->height_ += ps;
}

CMSPointXZ CMSBox::normlize(const GLoc3D& loc)
{
	return CMSPointXZ(loc.x() - left_top_.x(), loc.z() - left_top_.z());
}
