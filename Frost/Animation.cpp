#define FROST_EXPORT
#include "Animation.h"

namespace Frost
{
	namespace Graphics3D
	{
		Animation::Animation(void)
		{
		}

		Animation::~Animation(void)
		{
		}

		void Animation::setLength(double Length)
		{
			this->Length = Length;
		}

		double Animation::getLength()
		{
			return Length;
		}
	};
};