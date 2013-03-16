package league.of.yokos;

import java.util.List;
import java.util.Vector;

/**
 * Contains the basic content that each element (or node) in an XML document may contain.
 */
public class XmlElement
{
	public XmlElement()
	{
		Name = "";
		Text = "";
		Attributes = new Vector();
		SubElements = new Vector();
	}
	
	public String Name;
	public String Text;
	public Vector<XmlAttribute> Attributes;
	public Vector<XmlElement> SubElements;
	
	public XmlAttribute getAttribute(String attribName)
	{
		for (int i = 0; i < Attributes.size(); i++)
		{
			XmlAttribute x = Attributes.get(i);
			if (x.Name.equals(attribName))
			{
				return x;
			}
		}
		return null;
	}
	
	public void setAttribute(String attribName, String attribValue)
	{
		XmlAttribute attrib = getAttribute(attribName);
		
		if (attrib != null)
		{
			attrib.Value = attribValue;
		}
		else
		{
			attrib = new XmlAttribute();
			attrib.Name = attribName;
			attrib.Value = attribValue;
			Attributes.add(attrib);
		}
	}
	
	public XmlElement getElement(String nodeName)
	{
		for (int i = 0; i < SubElements.size(); i++)
		{
			XmlElement e = SubElements.get(i);
			if (e.Name.equals(nodeName))
			{
				return e;
			}
		}
		return null;
	}
	
	public void addElement(XmlElement elem)
	{
		SubElements.add(elem);
	}
	
	public String toString(int TabLevel)
	{
		if (!Name.isEmpty())
		{
			// Beginning of the tag.
			String ret = "";
			String TabString = "";

			for (int i = 0; i < TabLevel; i++)
			{
				TabString += '\t';
			}
			
			ret += TabString + "<" + Name;
			if (Attributes.size() > 0)
			{
				for (int i = 0; i < Attributes.size(); i++)
				{
					ret += " " + Attributes.get(i).Name + "=\"" + Attributes.get(i).Value + "\"";
				}
			}
			ret += ">";

			if (Text.isEmpty())
			{
				ret += '\n';
			}
			
			// Inner tag content.
			
			if (!Text.isEmpty()) // This tag just has plain text inside it.
			{
				ret += Text;
			}
			else // This tag has inner tags.
			{
				for (int i = 0; i < SubElements.size(); i++)
				{
					ret += SubElements.get(i).toString(TabLevel + 1) + '\n';
				}
			}

			// End of the tag.

			if (Text.isEmpty())
			{
				ret += TabString;
			}
			
			ret += "</" + Name + ">";
			
			// Return the fully serialized tag.
			return ret;
		}
		else
		{
			return "";
		}
	}
	
	@Override
	public String toString()
	{
		return toString(0);
	}
}
