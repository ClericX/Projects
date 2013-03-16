#pragma once

#include "DLL.h"

#include "MetricValueType.h"

namespace Frost
{
	class FROSTAPI MetricValue
	{
	public:
		MetricValue(void);
		MetricValue(MetricValueType *mvt, float amnt);
		~MetricValue(void);

		MetricValueType *type;
		float amount;

		MetricValue &operator = (MetricValue &v);
		bool operator == (MetricValue &v);
		bool operator != (MetricValue &v);

		void convertTo(MetricValueType *mvt);
		MetricValue getConvertedTo(MetricValueType *mvt);
	};
};