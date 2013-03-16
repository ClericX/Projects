#pragma once

#include "DLL.h"

#include "File.h"
#include "List.h"
#include "String.h"
#include "StringList.h"

namespace Frost
{
	namespace XML
	{
		// This class is for defining entities that should be replaced during serialization/deserialization of XML files.
		class FROSTAPI Entity
		{
		public:
			Entity(void);
			Entity(String theValue, String theEntity);
			~Entity(void);

			String value;
			String entity;
		};

		// Linked list of all loaded/specified entities to replace.
		//extern List<Entity *> Entities;
		//extern bool LoadedCoreEntities;

		void addEntity(String val, String ent);

		// This class is just for simple name/value pairs of data within elements.
		class FROSTAPI Attribute
		{
		public:
			Attribute(void);
			~Attribute(void);

			String Name;
			String Value;
		};

		// This class contains the basic content that each element may contain.
		class FROSTAPI Element
		{
		public:
			Element(void);
			~Element(void);

			String Name;
			String Text;
			List<Attribute *> Attributes;
			List<Element *> SubElements;

			Attribute *getAttribute(String attributeName);
			void setAttribute(String attributeName, String attributeValue);
			Attribute *getAttributeByValue(String attributeValue);
			Element *getNode(String nodeName);
			void addNode(Element *aNode);
			String entityEncode(String target);
			String toString(unsigned int TabLevel);
			String toString();
		};

		// This class handles reading and parsing existing files and also serializing data into new files.
		// TODO: Accept UTF-8 format files. Not necessary but enables multi-lang support.
		class FROSTAPI Document
		{
		public:
			Document(void);
			~Document(void);

			// The root XML element that the rest of the XML nodes branch from.
			Element *DocumentElement;

			// This function acquires an element in the node tree.
			Element *GetNode(String ElementName);

			// This function serializes the whole document node tree into an XML file format string.
			String toString();

			// This function saves the current state of the DOM into an XML file.
			void Save(String file);

			// This function handles opening the file and general parsing non-within tags.
			void Load(String file);

			// This function finds a tag to begin reading the node tree of.
			// It should be the first thing that is called after reading a file to start the chain of function
			// calls that result in fully parsing an XML format file into the DOM.
			Element *FindNextTag();

			// This function reads the properties of a tag.
			// The area that defines the tag name and it's properties.
			void ReadTagProperties(Element *theTag);

			// This function actually reads what's within the tag.
			// This includes inner tags and other content; it's used recursively.
			void ReadTagContents(Element *theTag);

			// Function to decode entities.
			String entityDecode(String target);

			// Function to check if end of data stream for XML parsing has been reached.
			bool StillReadingFile();

		private:
			String FileContent;
			unsigned int FilePos;
			String FileParseErrors;
			unsigned int NumTags;
		};
	};
};