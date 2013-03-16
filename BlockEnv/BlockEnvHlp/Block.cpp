#define FROST_EXPORT
#include "Block.h"
#include "../Frost/XML.h"

namespace Block
{
	TypeInfo::TypeInfo(void)
	{
		ID = 0;
		Destructability = 0;
		TheTexture = 0;
	}

	TypeInfo::~TypeInfo(void)
	{
	}

	Instance::Instance(void)
	{
		Type = 0;
	}

	Instance::~Instance(void)
	{
	}

	List<TypeInfo *> LoadedTypeInfo;
	List<Instance *> WorldBlockList;

	void Add(Instance *theBlock)
	{
		WorldBlockList.Add(theBlock);
	}

	bool Remove(Vector3f Location)
	{
		for (unsigned int i = 0; i < WorldBlockList.count(); i++)
		{
			if (WorldBlockList[i]->Position == Location)
			{
				WorldBlockList.Remove(i);
				return true;
			}
		}
		return false;
	}

	Instance *Get(Vector3f Location)
	{
		for (unsigned int i = 0; i < WorldBlockList.count(); i++)
		{
			if (WorldBlockList[i]->Position == Location)
			{
				return WorldBlockList[i];
			}
		}
		return (Instance *)0;
	}

	Vector3f AbsCoordToGridCoord(Vector3f absCoord)
	{
		Vector3f ret = absCoord;
		ret.x = floor(ret.x /= 10);
		ret.y = floor(ret.y /= 10);
		ret.z = floor(ret.z /= 10);
		return ret;
	}

	Vector3f GridCoordToAbsCoord(Vector3f gridCoord)
	{
		Vector3f ret = gridCoord;
		ret.x *= 10;
		ret.y *= 10;
		ret.z *= 10;
		return ret;
	}

	void LoadAllBlockTypeInfo()
	{
		XmlDocument blockXML;

		blockXML.Load("blocks.xml");

		for (unsigned int i = 0; i < blockXML.DocumentElement->SubElements.count(); i++)
		{
			XmlElement *typeElem = blockXML.DocumentElement->SubElements[i];
			TypeInfo *typeInfo = new TypeInfo();
			typeInfo->Name = typeElem->getNode("Name")->Text;
			typeInfo->ID = typeElem->getNode("ID")->Text.toInt();
			typeInfo->Destructability = typeElem->getNode("Destructability")->Text.toInt();
			typeInfo->TheTexture = new Texture();
			typeInfo->TheTexture->ImgFile = new File(typeElem->getNode("Filename")->Text);
			LoadedTypeInfo.Add(typeInfo);
		}

		Log::Msg("Successfully loaded all block type info.", Log::Info);
	}

	void LoadAllBlockTypeTextures()
	{
		for (unsigned int i = 0; i < LoadedTypeInfo.count(); i++)
		{
			LoadedTypeInfo[i]->TheTexture->Load();
		}
	}

	TypeInfo *GetTypeInfoByID(unsigned int ID)
	{
		for (unsigned int i = 0; i < LoadedTypeInfo.count(); i++)
		{
			if (LoadedTypeInfo[i]->ID == ID)
			{
				return LoadedTypeInfo[i];
			}
		}
		return (TypeInfo *)0;
	}

	TypeInfo *GetTypeInfoByName(String Name)
	{
		for (unsigned int i = 0; i < LoadedTypeInfo.count(); i++)
		{
			if (LoadedTypeInfo[i]->Name == Name)
			{
				return LoadedTypeInfo[i];
			}
		}
		return (TypeInfo *)0;
	}
};