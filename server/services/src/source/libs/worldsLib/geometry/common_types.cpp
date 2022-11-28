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

//---------------------------------------------CMSPointXY---------------------------------
const CMSPointXY& CMSPointXY::zero_point()
{
	static CMSPointXY zero(0.0, 0.0);
	return zero;
}

CMSPointXY& CMSPointXY::operator = (const CMSPointXY& v)
{
	this->x_ = v.x();
	this->y_ = v.y();

	return *this;
}

//{000,000}
bool CMSPointXY::build_from_str(std::string& str, CMSPointXY& p)
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
	p.set_y(dat[1]);

	return true;
}

std::string CMSPointXY::to_string()
{
	return ShareUtil::str_format<128>("{%10.3f,%10.3f}", x_, y_);
}

//---------------------------------------------CMSBox---------------------------------
CMSBox::CMSBox():left_top_( CMSPointXY::zero_point()), width_(0.0), height_(0.0)
{
}

CMSBox::CMSBox(float x, float y, float w, float h):left_top_(x, y), width_( w), height_( y)
{
}

void CMSBox::init_box(const CMSPointXY& pos, float w, float h)
{
	left_top_ = pos;
	width_ = w;
	height_ = h;
}

bool CMSBox::is_point_in_box(const CMSPointXY& p)
{
	float dx = p.x() - left_top_.x();
	float dy = left_top_.y() - p.y();
	return (dx >= 0 && dx < width_) && (dy > 0 && dy <= height_);
}

CMSBox& CMSBox::operator = (const CMSBox& v)
{
	this->left_top_ = v.left_top_;
	this->width_ = v.width_;
	this->height_ = v.height_;

	return *this;
}