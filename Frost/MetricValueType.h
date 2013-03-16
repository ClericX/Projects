#pragma once

#include "DLL.h"

#include "String.h"

namespace Frost
{
	class FROSTAPI MetricValueType
	{
	public:
		MetricValueType(void);
		MetricValueType(String n, String sn, float scale);
		~MetricValueType(void);

		String Name;
		String shortName;
		float ScalingWithUnits;

		bool operator == (MetricValueType &mvt);
		bool operator != (MetricValueType &mvt);

		// Specific metric types.
		static MetricValueType Centimeters;
		static MetricValueType Inches;
		static MetricValueType Meters;
		static MetricValueType Kilometers;
		static MetricValueType Millimeters;
		static MetricValueType FrostUnits;
		static MetricValueType Pixels;

		static float UnitToPixelRatio;

	private:
		static int LastAssignedTypeID; // Assign one per metric and increment it. For comparison reasons.
		int AssignedTypeID;
		int getTypeID();
	};
};