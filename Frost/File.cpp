#define FROST_EXPORT
#include "File.h"

#include <errno.h>
#include "Log.h"

namespace Frost
{
	File::File(void)
	{
		Init();
	}

	File::File(String _Path)
	{
		Init();
		Path = _Path;
	}

	File::File(String _Path, AccessType _OpenMode)
	{
		Init();
		Path = _Path;
		OpenMode = _OpenMode;
	}

	File::~File(void)
	{
		for (unsigned int i = 0; i < LoadedFiles.count(); i++)
		{
			if (LoadedFiles[i] == this)
			{
				LoadedFiles.Remove(i);
				break;
			}
		}
	
		if (IsOpen())
		{
			Close();
		}
	}

	void File::Init()
	{
		f = 0;
		Size = 0;
		OpenMode = ReadOnlyBinary;
		LoadedFiles.Add(this);
	}

	int File::Open(AccessType typemode)
	{
		String mode = getAccessTypeString(typemode);

		// Check if we have enough info to open the file properly.
		if (Path.empty() || mode.empty())
		{
			String errormsg;
			errormsg << "Attempted to open a file without specifying either path or open mode.\n";
			errormsg << "Path: " << Path << "\n";
			errormsg << "O.M.: " << mode;
			Log::Msg(errormsg, Log::FatalError);
			return -1;
		}

		// If we can attempt to open the file, continue to do so.
		return fopen_s(&f, Path.c_str(), mode.c_str());
	}

	void File::Open()
	{
		int err = Open(OpenMode);
		if (err != 0)
		{
			String errormsg;

			switch (err)
			{
			case EACCES:
				errormsg << "Permission was denied to open the file \"" << Path << "\"";
				break;
			case EMFILE:
				errormsg << "The file handle for \"" << Path << "\" could not be acquired. Too many files may be open.";
				break;
			case ENOENT:
				errormsg << "The file \"" << Path << "\" does not exist.";
				break;
			}
		
			Log::Msg(errormsg, Log::FatalError);
		}
		else
		{
			getFilesize();
		}
	}

	void File::Seek(long offset, int origin)
	{
		int res = fseek(f, offset, origin);
		if (res != 0)
		{
			String errormsg;
			errormsg << "Failed to seek to position " << offset << " in file " << Path << " starting from " << origin;
			Log::Msg(errormsg, Log::FatalError);
		}
	}

	void File::Seek(long offset)
	{
		Seek(offset, SeekPosition::Current);
	}

	unsigned char File::readByte()
	{
		char c = 0;
		if (fread(&c, 1, 1, f) == 1)
		{
			return c;
		}
		else
		{
			String errormsg;
			errormsg << "Failed to read data from \"" << Path << "\"\nReason: ";

			if (eof())
			{
				errormsg << "EOF";
			}
			else if (error())
			{
				errormsg << "I/O Error";
			}

			Log::Msg(errormsg, Log::FatalError);

			return (unsigned char)0;
		}
	}

	void File::writeByte(unsigned char c)
	{
		if (fwrite(&c, 1, 1, f) != 1)
		{
			String errormsg;
			errormsg << "Failed to write data to \"" << Path << "\"\nReason: ";

			if (eof())
			{
				errormsg << "EOF";
			}
			else if (error())
			{
				errormsg << "I/O Error";
			}

			Log::Msg(errormsg, Log::FatalError);
		}
	}

	bool File::eof()
	{
		return ( (feof(f) != 0) || (getFilePointerOffset() == Size) );
	}

	bool File::error()
	{
		return (ferror(f) != 0);
	}

	void File::rewind()
	{
		Seek(0, SeekPosition::Start);
		if (clearerr_s(f) != 0)
		{
			String errormsg;
			errormsg << "Failed to clear error indicators on stream for file \"" << Path << "\"";
			Log::Msg(errormsg, Log::FatalError);
		}
	}

	String File::readLine()
	{
		String ret;
		bool End = false;
		while (!End && !eof())
		{
			char c = (char)readByte();
			if (c == '\r' || c == '\n')
			{
				if (c == '\r')
				{
					char n = (char)readByte();
					if (n != '\n')
					{
						Seek(-1);
					}
				}
				End = true;
			}
			else
			{
				ret += c;
			}
		}
		return ret;
	}

	String File::readAllLines()
	{
		String ret;

		while (!eof())
		{
			ret += readLine();
			ret += '\n';
		}

		return ret;
	}

	void File::writeFile(String content)
	{
		for (unsigned int i = 0; i < content.length(); i++)
		{
			writeByte(content[i]);
		}
	}

	void File::writeLine(String theLine)
	{
		writeFile(theLine);
		writeByte('\n');
	}

	unsigned long File::getFilePointerOffset()
	{
		//return (unsigned long)(f->_ptr - f->_base);
		return (unsigned long)ftell(f);
	}

	void File::getFilesize()
	{
		// Get the filesize.
		Seek(0, SeekPosition::End);
		Size = getFilePointerOffset();
		Seek(0, SeekPosition::Start);
	}

	bool File::IsOpen()
	{
		return (f != 0);
	}

	void File::Close()
	{
		fclose(f);
		f = 0;
	}

	bool File::Exists()
	{
		if (Open(ReadOnly) != ENOENT)
		{
			Close();
			return true;
		}
		return false;
	}

	String File::getAccessTypeString(AccessType type)
	{
		switch (type)
		{
		case ReadOnly:
			return "r";
		case WriteOnly:
			return "w";
		case AppendOnly:
			return "a";
		case ReadWriteExisting:
			return "r+";
		case ReadWrite:
			return "w+";
		case ReadAppend:
			return "a+";
		case Text:
			return "t";
		case Binary:
			return "b";
		case ShortLived:
			return "T";
		case Temporary:
			return "D";
		case ReadWriteExistingBinary:
			return "rb+";
		case ReadOnlyBinary:
			return "rb";
		}
		return "";
	}

	List<File *> File::LoadedFiles;
};