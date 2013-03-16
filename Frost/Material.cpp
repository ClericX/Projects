#define FROST_EXPORT
#include "Material.h"

namespace Frost
{
	namespace Graphics3D
	{
		Material::Material(void)
		{
			DiffuseMap = 0;
		}

		Material::~Material(void)
		{
		}
	};
};