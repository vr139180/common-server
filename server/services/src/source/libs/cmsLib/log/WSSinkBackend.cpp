#include <cmsLib/log/WSSinkBackend.h>

#include <boost/log/expressions.hpp>
#include <cmsLib/util/ShareUtil.h>

WSSinkBackend::~WSSinkBackend()
{
}

void WSSinkBackend::consume(logging::record_view const& rec)
{
	const std::string& msg = *rec[logging::expressions::smessage];
	if (rec.attribute_values().count("Model") > 0)
	{
		logging::attribute_value model = rec.attribute_values()["Model"];
	}
}

void WSSinkBackend::flush()
{

}

void WSSinkBackend::add_model_url(std::string model, std::string url)
{
	ShareUtil::to_lower(model);
	models_[model] = url;
}
