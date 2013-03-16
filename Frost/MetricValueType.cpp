#define FROST_EXPORT
#include "MetricValueType.h"

namespace Frost
{
	float MetricValueType::UnitToPixelRatio = 0.1f;

	MetricValueType::MetricValueType(void)
	{
		ScalingWithUnits = 0;
		AssignedTypeID = 0;
	}

	MetricValueType::MetricValueType(String n, String sn, float scale)
	{
		Name = n;
		shortName = sn;
		ScalingWithUnits = scale;
		AssignedTypeID = 0;
	}

	MetricValueType::~MetricValueType(void)
	{
	}

	bool MetricValueType::operator == (MetricValueType &mvt)
	{
		return (this->getTypeID() == mvt.getTypeID());
	}

	bool MetricValueType::operator != (MetricValueType &mvt)
	{
		return !((*this) == mvt);
	}
	
	// Specific metric types.
	MetricValueType Centimeters = MetricValueType("Centimeters", "cm", 1);
	MetricValueType Inches = MetricValueType("Inches", "in", 1);
	MetricValueType Meters = MetricValueType("Meters", "m", 1);
	MetricValueType Kilometers = MetricValueType("Kilometers", "km", 1);
	MetricValueType Millimeters = MetricValueType("Millimeters", "mm", 1);
	MetricValueType FrostUnits = MetricValueType("Frost Units", "fx", 1);
	MetricValueType Pixels = MetricValueType("Pixels", "px", 1);

	int MetricValueType::LastAssignedTypeID = 0;

	int MetricValueType::getTypeID()
	{
		if (AssignedTypeID == 0)
		{
			LastAssignedTypeID++;
			AssignedTypeID = LastAssignedTypeID;
		}
			
		return AssignedTypeID;
	}
};