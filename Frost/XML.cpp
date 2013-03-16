#define FROST_EXPORT
#include "XML.h"

namespace Frost
{
	namespace XML
	{
		Entity::Entity(void)
		{
		}

		Entity::Entity(String theValue, String theEntity)
		{
			value = theValue;
			entity = theEntity;
		}

		Entity::~Entity(void)
		{
		}

		List<Entity *> Entities;
		bool LoadedCoreEntities = false;

		void addEntity(String val, String ent)
		{
			Entities.Add(new Entity(val, ent));
		}

		Attribute::Attribute(void)
		{
		}

		Attribute::~Attribute(void)
		{
		}

		Element::Element(void)
		{
		}

		Element::~Element(void)
		{
		}

		Attribute *Element::getAttribute(String attributeName)
		{
			for (unsigned int i = 0; i < Attributes.count(); i++)
			{
				if (Attributes[i]->Name == attributeName)
				{
					return Attributes[i];
				}
			}
			return (Attribute *)0;
		}

		void Element::setAttribute(String attributeName, String attributeValue)
		{
			Attribute *Attrib = getAttribute(attributeName);

			if (Attrib != 0)
			{
				Attrib->Value = attributeValue;
			}
			else
			{
				Attrib = new Attribute();
				Attrib->Name = attributeName;
				Attrib->Value = attributeValue;
				Attributes.Add(Attrib);
			}
		}

		Attribute *Element::getAttributeByValue(String attributeValue)
		{
			for (unsigned int i = 0; i < Attributes.count(); i++)
			{
				if (Attributes[i]->Value == attributeValue)
				{
					return Attributes[i];
				}
			}
			return (Attribute *)0;
		}

		Element *Element::getNode(String nodeName)
		{
			for (unsigned int i = 0; i < SubElements.count(); i++)
			{
				if (SubElements[i]->Name == nodeName)
				{
					return SubElements[i];
				}
			}
			return (Element *)0;
		}

		void Element::addNode(Element *aNode)
		{
			SubElements.Add(aNode);
		}

		String Element::entityEncode(String target)
		{
			for (unsigned int i = 0; i < Entities.count(); i++)
			{
				Entity *ent = Entities[i];
				target.replaceAll(ent->value, String("&") + ent->entity + ";");
			}

			return target;
		}

		String Element::toString(unsigned int TabLevel)
		{
			if (!Name.empty())
			{
				// Beginning of the tag.
				String ret;
				String TabString;

				for (unsigned int i = 0; i < TabLevel; i++)
				{
					TabString += '\t';
				}
			
				ret << TabString << "<" << entityEncode(Name);
				if (Attributes.count() > 0)
				{
					for (unsigned int i = 0; i < Attributes.count(); i++)
					{
						ret << " " << entityEncode(Attributes[i]->Name) << "=\"" << entityEncode(Attributes[i]->Value) << "\"";
					}
				}
				ret << ">";

				if (Text.empty())
				{
					ret << '\n';
				}
			
				// Inner tag content.
			
				if (!Text.empty()) // This tag just has plain text inside it.
				{
					ret << entityEncode(Text);
				}
				else // This tag has inner tags.
				{
					for (unsigned int i = 0; i < SubElements.count(); i++)
					{
						ret.addLine(SubElements[i]->toString(TabLevel + 1));
					}
				}

				// End of the tag.

				if (Text.empty())
				{
					ret << TabString;
				}
			
				ret << "</" << entityEncode(Name) << ">";
			
				// Return the fully serialized tag.
				return ret;
			}
			else
			{
				return "";
			}
		}

		String Element::toString()
		{
			return toString(0);
		}

		Document::Document(void)
		{
			DocumentElement = new Element();
			FilePos = 0;
			NumTags = 0;

			if (!LoadedCoreEntities)
			{
				// & symbol must be entity'd first
				addEntity("&", "amp");
				// Then the rest can come.
				addEntity("<", "lt");
				addEntity(">", "gt");
				addEntity("'", "apos");
				addEntity("\"", "quot");

				LoadedCoreEntities = true;
			}
		}

		Document::~Document(void)
		{
			delete DocumentElement;
		}

		Element *Document::GetNode(String ElementName)
		{
			StringList subElems = ElementName.split(".");
			Element *retVal = DocumentElement;

			for (unsigned int i = 0; i < subElems.count(); i++)
			{
				if (retVal != 0)
				{
					retVal = retVal->getNode(subElems[i]);
				}
				else
				{
					Log::Msg(String("Attempted to get an FXML node that does not exist. Request String: ") + ElementName, Log::FatalError);
				}
			}

			return retVal;
		}

		String Document::toString()
		{
			String writeContent;

			for (unsigned int i = 0; i < DocumentElement->SubElements.count(); i++)
			{
				writeContent += DocumentElement->SubElements[i]->toString(0);
				writeContent << '\n' << '\n';
			}

			return writeContent;
		}

		void Document::Save(String file)
		{
			File FileHandle;
			FileHandle.Path = file;
			FileHandle.OpenMode = File::WriteOnly;
			FileHandle.Open();
			FileHandle.writeFile(toString());
			FileHandle.Close();
		}

		void Document::Load(String file)
		{
			File FileHandle;
			FileHandle.Path = file;
			FileHandle.OpenMode = File::ReadOnlyBinary;
			FileHandle.Open();
			FileContent = FileHandle.readAllLines();
			FileHandle.Close();

			while (StillReadingFile())
			{
				// This does not check if there are no tags in the file at all.
				// There are also no checks if the file actually has any content.
				// However, upon an error occurring the program will terminate.
				Element *potentialElement = FindNextTag();
				if (potentialElement != 0)
				{
					DocumentElement->SubElements.Add(potentialElement);
				}
			}

			if (FileContent.empty())
			{
				FileParseErrors.addLine("The file was empty.");
			}

			// If errors occurred in parsing the file, this must be dealt with.
			if (!FileParseErrors.empty())
			{
				String ParseError;
				ParseError << "The file \"" << file << "\" has errors in the XML formatting. The error(s) are:" << '\n' << FileParseErrors;
				Log::Msg(ParseError, Log::FatalError);
			}
			else
			{
				Log::Msg(String("Successfully loaded \"") + file + "\".", Log::Info);
			}
		}

		Element *Document::FindNextTag()
		{
			Element *retTag = new Element();

			bool FoundTagOpening = false;

			while (!FoundTagOpening && StillReadingFile())
			{
				if (FileContent[FilePos] == '<')
				{
					// Don't bother with comments.
					if (FileContent[FilePos + 1] == '!' && FileContent[FilePos + 2] == '-' && FileContent[FilePos + 3] == '-')
					{
						// Skip the characters we just checked for comment entrance.
						FilePos += 3;

						// Keep going through contents until the comment is closed.
						while (!(FileContent[FilePos - 2] == '-' && FileContent[FilePos - 1] == '-' && FileContent[FilePos] == '>'))
						{
							FilePos++;
						}
					}
					// Otherwise it's a valid tag.
					else
					{
						FoundTagOpening = true;
					}
				}
				FilePos++;
			}

			if (FoundTagOpening)
			{
				NumTags++;
				ReadTagProperties(retTag);
			}
			else if (NumTags == 0)
			{
				FileParseErrors.addLine("Searched the whole document and did not find a tag opening (<) anywhere. (Is the file the correct format?)");
				delete retTag;
				retTag = 0;
			}
			else
			{
				//FileParseErrors.addLine("Tried to read a tag where a tag was nonexistent. This is very likely a parser error.");
				// This really just means there is whitespace at the end of the file after the last tag has been closed, not really an error...
				delete retTag;
				retTag = 0;
			}

			return retTag;
		}

		void Document::ReadTagProperties(Element *theTag)
		{
			bool ReadTagName = false;
			bool ReadingTagName = false;
			bool ReadingPropName = false;
			bool ExpectingEquals = false;
			bool ReadingPropVal = false;
			bool ReadingTagProperties = true;
			bool ExpectingReadPropValStart = false;
			Attribute *curAttrib = 0;

			while (StillReadingFile() && ReadingTagProperties)
			{
				bool ProcessedSomething = false;
				if (FileContent[FilePos] == ' ' && ReadingTagName)
				{
					ReadingTagName = false;
					theTag->Name = entityDecode(theTag->Name);
					ReadTagName = true;
					ProcessedSomething = true;
				}
				if ((FileContent[FilePos] == ' ' || FileContent[FilePos] == '=') && ReadingPropName)
				{
					ReadingPropName = false;
					curAttrib->Name = entityDecode(curAttrib->Name);
					ExpectingEquals = true;
					ProcessedSomething = true;
				}
				if (FileContent[FilePos] == '=' && ExpectingEquals)
				{
					ExpectingEquals = false;
					ExpectingReadPropValStart = true;
					ProcessedSomething = true;
				}
				if (FileContent[FilePos] == '\"' && ExpectingReadPropValStart)
				{
					ExpectingReadPropValStart = false;
					ReadingPropVal = true;
					ProcessedSomething = true;
				}
				if (FileContent[FilePos] == '\"' && ReadingPropVal && !ProcessedSomething)
				{
					ReadingPropVal = false;
					curAttrib->Value = entityDecode(curAttrib->Value);
					theTag->Attributes.Add(curAttrib);
					curAttrib = 0;
					ProcessedSomething = true;
				}
				if (FileContent[FilePos] == '>')
				{
					// The tag ended.
					ReadingTagProperties = false;
					ProcessedSomething = true;
				}
				if (FileContent[FilePos] != ' ' && !ProcessedSomething)
				{
					if (!ReadTagName)
					{
						if (!ReadingTagName)
						{
							ReadingTagName = true;
						}
						theTag->Name += FileContent[FilePos];
					}
					else if (!ReadingPropName && !ReadingPropVal)
					{
						ReadingPropName = true;
					}

					if (ReadingPropName)
					{
						if (curAttrib == 0)
						{
							curAttrib = new Attribute();
						}
						curAttrib->Name += FileContent[FilePos];
					}
					else if (ReadingPropVal)
					{
						curAttrib->Value += FileContent[FilePos];
					}
				}

				FilePos++;
			}

			ReadTagContents(theTag);
		}

		void Document::ReadTagContents(Element *theTag)
		{
			bool AdditionalTagConflict = false;
			bool ReadingInnerTag = false;
			bool ReadingClosingTag = false;
			bool TagClosed = false;
			bool ReadingTagContents = true;
			String ClosingTagName;

			while (StillReadingFile() && ReadingTagContents)
			{
				if (FileContent[FilePos] == '<')
				{
					AdditionalTagConflict = true;
				}
				else if (AdditionalTagConflict)
				{
					if (FileContent[FilePos] == '/')
					{
						AdditionalTagConflict = false;
						ReadingClosingTag = true;
					}
					else
					{
						AdditionalTagConflict = false;
						ReadingInnerTag = true;
						FilePos--;
						theTag->SubElements.Add(FindNextTag());
					}
				}
				else if (ReadingClosingTag)
				{
					if (FileContent[FilePos] != ' ' && FileContent[FilePos] != '>')
					{
						ClosingTagName += FileContent[FilePos];
					}
					else if (FileContent[FilePos] == '>')
					{
						ReadingClosingTag = false;
						ClosingTagName = entityDecode(ClosingTagName);
						if (theTag->Name == ClosingTagName)
						{
							TagClosed = true;
						}
						else
						{
							FileParseErrors.addLine(String("The tag \"") + theTag->Name + "\" was not properly closed.");
							FileParseErrors.addLine(ClosingTagName);
						}
					}
				}
				else if (FileContent[FilePos] != '\t' && FileContent[FilePos] != ' ' && FileContent[FilePos] != '\n')
				{
					theTag->Text += FileContent[FilePos];
				}

				if (TagClosed)
				{
					ReadingTagContents = false;
					theTag->Text = entityDecode(theTag->Text);
				}

				if (!ReadingInnerTag)
				{
					FilePos++;
				}
				else
				{
					ReadingInnerTag = false;
				}
			}
		}

		String Document::entityDecode(String target)
		{
			unsigned int curEntIndex = 0;

			while (curEntIndex = target.find("&", curEntIndex) != (unsigned int)-1)
			{
				String readEnt;

				for (unsigned int i = curEntIndex + 1; i < target.length(); i++)
				{
					if (target.charAt(i) != ';')
					{
						readEnt += target.charAt(i);
					}
					else
					{
						break;
					}
				}

				Entity *foundEnt = 0;

				// Check backwards so the & entity is processed last.
				for (unsigned int i = Entities.count() - 1; i > 0; i--)
				{
					Entity *ent = Entities[i];
					
					if (ent->entity == readEnt)
					{
						foundEnt = ent;
						break;
					}
				}

				if (foundEnt == 0)
				{
					FileParseErrors.addLine(String("Undefined entity \"") + readEnt + "\" at pos " + FilePos + ".");
					return "x";
				}

				// full entity length is & + entlen + ;
				unsigned int fullEntLen = foundEnt->entity.length() + 2;
				// replace the string
				target.replace(curEntIndex, curEntIndex + fullEntLen, foundEnt->value);
				// subtract the current index based on size diff.
				curEntIndex -= (fullEntLen - foundEnt->value.length());
			}

			return target;
		}

		bool Document::StillReadingFile()
		{
			return (FilePos < FileContent.length() - 1) && FileParseErrors.empty();
		}
	};
};