#pragma once

#include "DLL.h"

#include "List.h"
#include "String.h"
#include "MetricValueType.h"
#include "MetricValue.h"

namespace Frost
{
	class FROSTAPI Metric
	{
	public:
		Metric(void);
		~Metric(void);

		void setValue(String valStr);
		void setValue(MetricValue &mv);
		void addValue(MetricValue &mv);
		MetricValue *getValue(MetricValueType *mvt);
		void MergeToType(MetricValueType *mvt);
		Metric getAsType(MetricValueType *mvt);

	private:
		List<MetricValue *> valList;
	};
};