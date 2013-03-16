#define FROST_EXPORT
#include "Metric.h"

namespace Frost
{
	Metric::Metric(void)
	{
	}

	Metric::~Metric(void)
	{
	}

	void Metric::setValue(String valStr)
	{
		// TODO: implement
	}

	void Metric::setValue(MetricValue &mv)
	{
		MetricValue *existing = getValue(mv.type);

		if (existing != 0)
		{
			existing->amount = mv.amount;
		}
		else
		{
			valList.Add(new MetricValue(mv));
		}
	}

	void Metric::addValue(MetricValue &mv)
	{
		MetricValue *existing = getValue(mv.type);

		if (existing != 0)
		{
			existing->amount += mv.amount;
		}
		else
		{
			valList.Add(new MetricValue(mv));
		}
	}

	MetricValue *Metric::getValue(MetricValueType *mvt)
	{
		for (unsigned int i = 0; i < valList.count(); i++)
		{
			if ((*valList[i]->type) == (*mvt))
			{
				return valList[i];
			}
		}
		return (MetricValue *)0;
	}

	void Metric::MergeToType(MetricValueType *mvt)
	{
		MetricValue *final = new MetricValue();
		final->type = mvt;

		for (unsigned int i = 0; i < valList.count(); i++)
		{
			MetricValue *current = valList[i];
			final->amount += current->getConvertedTo(mvt).amount;
			delete current;
		}

		valList.clear();
		valList.Add(final);
	}

	Metric Metric::getAsType(MetricValueType *mvt)
	{
		Metric ret(*this);
		ret.MergeToType(mvt);
		return ret;
	}
};