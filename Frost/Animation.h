#pragma once

#include "DLL.h"

#include "String.h"
#include "NamedObject.h"

namespace Frost
{
	namespace Graphics3D
	{
		class FROSTAPI Animation : public NamedObject
		{
		public:
			Animation(void);
			~Animation(void);

			double Length;

			void setLength(double Length);
			double getLength();
		};
	};
};