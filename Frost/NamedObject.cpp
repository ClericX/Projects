#define FROST_EXPORT
#include "NamedObject.h"

namespace Frost
{
	NamedObject::NamedObject(void)
	{
	}

	NamedObject::~NamedObject(void)
	{
	}

	void NamedObject::setName(String Name)
	{
		this->Name = Name;
	}

	String NamedObject::getName()
	{
		return this->Name;
	}

	bool NamedObject::HasName()
	{
		return this->Name.length() > 0;
	}
};