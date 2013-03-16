#define FROST_EXPORT
#include "MetricValue.h"

namespace Frost
{
	MetricValue::MetricValue(void)
	{
		type = 0;
		amount = 0.0f;
	}

	MetricValue::MetricValue(MetricValueType *mvt, float amnt)
	{
		type = mvt;
		amount = amnt;
	}

	MetricValue::~MetricValue(void)
	{
	}

	MetricValue &MetricValue::operator = (MetricValue &v)
	{
		type = v.type;
		amount = v.amount;
		return (*this);
	}

	bool MetricValue::operator == (MetricValue &v)
	{
		return (v.type == this->type && v.amount == this->amount);
	}

	bool MetricValue::operator != (MetricValue &v)
	{
		return !(v == (*this));
	}

	void MetricValue::convertTo(MetricValueType *mvt)
	{
		float valUnits = amount * type->ScalingWithUnits;
		amount = valUnits / mvt->ScalingWithUnits;
		type = mvt;
	}

	MetricValue MetricValue::getConvertedTo(MetricValueType *mvt)
	{
		MetricValue ret(*this);
		ret.convertTo(mvt);
		return ret;
	}
};