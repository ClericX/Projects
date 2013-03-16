#pragma once

#include "DLL.h"
#include "../Frost/List.h"
#include "../Frost/Vector3f.h"
#include "../Frost/File.h"
#include "../Frost/Texture.h"

namespace Block
{
	class FROSTAPI TypeInfo
	{
	public:
		TypeInfo(void);
		~TypeInfo(void);

		String Name;
		unsigned int ID;
		unsigned int Destructability;
		Texture *TheTexture;
	};

	class FROSTAPI Instance
	{
	public:
		Instance(void);
		~Instance(void);

		Vector3f Position;
		TypeInfo *Type;
	};

	extern "C" FROSTAPI List<TypeInfo *> LoadedTypeInfo;
	extern "C" FROSTAPI List<Instance *> WorldBlockList;

	void FROSTAPI Add(Instance *theBlock);
	bool FROSTAPI Remove(Vector3f Location);
	Instance FROSTAPI *Get(Vector3f Location);

	Vector3f FROSTAPI AbsCoordToGridCoord(Vector3f absCoord);
	Vector3f FROSTAPI GridCoordToAbsCoord(Vector3f gridCoord);

	void FROSTAPI LoadAllBlockTypeInfo();
	void FROSTAPI LoadAllBlockTypeTextures();
	TypeInfo FROSTAPI *GetTypeInfoByID(unsigned int ID);
	TypeInfo FROSTAPI *GetTypeInfoByName(String Name);
};