package league.of.yokos;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;

public class XmlDocument extends XmlElement
{
	public XmlDocument()
	{
		FileContent = "";
		FilePos = 0;
		FileParseErrors = "";
	}
	
	public void Load(String path)
	{
		try
		{
			BufferedReader in = new BufferedReader(new FileReader(path));
			String ln = in.readLine();
			
			while (ln != null)
			{
				FileContent += ln;
				ln = in.readLine();
			}
			
			in.close();
		}
		catch (Exception e)
		{
			System.out.println("Failed to load the XML file \"" + path + "\".");
		}
		
		while (StillReadingFile())
		{
			XmlElement potentialElement = FindNextTag();
			if (potentialElement != null)
			{
				SubElements.add(potentialElement);
			}
		}
	}
	
	public void Save(String path)
	{
		try
		{
			BufferedWriter in = new BufferedWriter(new FileWriter(path));
			String writeContent = "";

			for (int i = 0; i < SubElements.size(); i++)
			{
				writeContent += SubElements.get(i).toString();
				writeContent += "\n\n";
			}

			in.write(writeContent);
			in.close();
		}
		catch (Exception e)
		{
			System.out.println("Failed to save an XML file.");
		}
	}
	
	private String FileContent;
	private int FilePos;
	private String FileParseErrors;
	
	private boolean StillReadingFile()
	{
		return (FilePos < FileContent.length() - 1);
	}
	
	private XmlElement FindNextTag()
	{
		XmlElement retTag = new XmlElement();
		
		boolean FoundTagOpening = false;
		
		while (!FoundTagOpening && StillReadingFile())
		{
			if (FileContent.charAt(FilePos) == '<')
			{
				FoundTagOpening = true;
			}
			FilePos++;
		}
		
		if (FoundTagOpening)
		{
			ReadTagProperties(retTag);
		}
		else if (SubElements.isEmpty())
		{
			System.out.println("Tried to load an XML with no tags in it.");
		}
		else
		{
			return null;
		}
		
		return retTag;
	}
	
	private void ReadTagProperties(XmlElement theTag)
	{
		boolean ReadTagName = false;
		boolean ReadingTagName = false;
		boolean ReadingPropName = false;
		boolean ExpectingEquals = false;
		boolean ReadingPropVal = false;
		boolean ReadingTagProperties = true;
		boolean ExpectingReadPropValStart = false;
		XmlAttribute curAttrib = null;

		while (StillReadingFile() && ReadingTagProperties)
		{
			boolean ProcessedSomething = false;
			if (FileContent.charAt(FilePos) == ' ' && ReadingTagName)
			{
				ReadingTagName = false;
				ReadTagName = true;
				ProcessedSomething = true;
			}
			if ((FileContent.charAt(FilePos) == ' ' || FileContent.charAt(FilePos) == '=') && ReadingPropName)
			{
				ReadingPropName = false;
				ExpectingEquals = true;
				ProcessedSomething = true;
			}
			if (FileContent.charAt(FilePos) == '=' && ExpectingEquals)
			{
				ExpectingEquals = false;
				ExpectingReadPropValStart = true;
				ProcessedSomething = true;
			}
			if (FileContent.charAt(FilePos) == '\"' && ExpectingReadPropValStart)
			{
				ExpectingReadPropValStart = false;
				ReadingPropVal = true;
				ProcessedSomething = true;
			}
			if (FileContent.charAt(FilePos) == '\"' && ReadingPropVal && !ProcessedSomething)
			{
				ReadingPropVal = false;
				theTag.Attributes.add(curAttrib);
				curAttrib = null;
				ProcessedSomething = true;
			}
			if (FileContent.charAt(FilePos) == '>')
			{
				// The tag ended.
				ReadingTagProperties = false;
				ProcessedSomething = true;
			}
			if (FileContent.charAt(FilePos) != ' ' && !ProcessedSomething)
			{
				if (!ReadTagName)
				{
					if (!ReadingTagName)
					{
						ReadingTagName = true;
					}
					theTag.Name += FileContent.charAt(FilePos);
				}
				else if (!ReadingPropName && !ReadingPropVal)
				{
					ReadingPropName = true;
				}

				if (ReadingPropName)
				{
					if (curAttrib == null)
					{
						curAttrib = new XmlAttribute();
					}
					curAttrib.Name += FileContent.charAt(FilePos);
				}
				else if (ReadingPropVal)
				{
					curAttrib.Value += FileContent.charAt(FilePos);
				}
			}

			FilePos++;
		}

		ReadTagContents(theTag);
	}
	
	private void ReadTagContents(XmlElement theTag)
	{
		boolean AdditionalTagConflict = false;
		boolean ReadingInnerTag = false;
		boolean ReadingClosingTag = false;
		boolean TagClosed = false;
		boolean ReadingTagContents = true;
		String ClosingTagName = "";

		while (StillReadingFile() && ReadingTagContents)
		{
			if (FileContent.charAt(FilePos) == '<')
			{
				AdditionalTagConflict = true;
			}
			else if (AdditionalTagConflict)
			{
				if (FileContent.charAt(FilePos) == '/')
				{
					AdditionalTagConflict = false;
					ReadingClosingTag = true;
				}
				else
				{
					AdditionalTagConflict = false;
					ReadingInnerTag = true;
					FilePos--;
					theTag.SubElements.add(FindNextTag());
				}
			}
			else if (ReadingClosingTag)
			{
				if (FileContent.charAt(FilePos) != ' ' && FileContent.charAt(FilePos) != '>')
				{
					ClosingTagName += FileContent.charAt(FilePos);
				}
				else if (FileContent.charAt(FilePos) == '>')
				{
					ReadingClosingTag = false;
					if (theTag.Name.equals(ClosingTagName))
					{
						TagClosed = true;
					}
					else
					{
						FileParseErrors += ("The tag \"" + theTag.Name + "\" was not properly closed." + '\n');
						FileParseErrors += (ClosingTagName + '\n');
					}
				}
			}
			else if (FileContent.charAt(FilePos) != '\t' && FileContent.charAt(FilePos) != '\n')
			{
				theTag.Text += FileContent.charAt(FilePos);
			}

			if (TagClosed)
			{
				ReadingTagContents = false;
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
}
