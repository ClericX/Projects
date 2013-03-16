#pragma once

#include "DLL.h"
#include <Windows.h>
#include <stdio.h>
#include "String.h"
#include "List.h"

namespace Frost
{
	namespace SeekPosition
	{
		static const int Start = SEEK_SET;
		static const int Current = SEEK_CUR;
		static const int End = SEEK_END;
	};

	class FROSTAPI File
	{
	public:
		enum AccessType
		{
			// Standard Flags.
			ReadOnly,			// Opens for reading. If the file does not exist or cannot be found, the fopen call fails.
			WriteOnly,			// Opens an empty file for writing. If the given file exists, its contents are destroyed.
			AppendOnly,			// Opens for writing at the end of the file (appending) without removing the EOF marker before writing new data to the file; creates the file first if it doesn't exist.
			ReadWriteExisting,	// Opens for both reading and writing. (The file must exist.)
			ReadWrite,			// Opens an empty file for both reading and writing. If the given file exists, its contents are destroyed.
			ReadAppend,			// Opens for reading and appending; the appending operation includes the removal of the EOF marker before new data is written to the file and the EOF marker is restored after writing is complete; creates the file first if it doesn't exist.
			Text,				// Open in text (translated) mode. In this mode, CTRL+Z is interpreted as an end-of-file character on input.
			Binary,				// Open in binary (untranslated) mode; translations involving carriage-return and linefeed characters are suppressed.
			ShortLived,			// Specifies a file as temporary. If possible, it is not flushed to disk.
			Temporary,			// Specifies a file as temporary. It is deleted when the last file pointer is closed.

			// Ease-of-use.
			ReadWriteExistingBinary,
			ReadOnlyBinary
		};

		File(void);
		File(String _Path);
		File(String _Path, AccessType _OpenMode);
		~File(void);

		FILE *f;
		String Path;
		AccessType OpenMode;
		unsigned long Size;

		void Init();
		int Open(AccessType typemode);
		void Open();
		void Seek(long offset, int origin);
		void Seek(long offset);
		unsigned char readByte();
		void writeByte(unsigned char c);
		bool eof();
		bool error();
		void rewind();
		String readLine();
		String readAllLines();
		void writeFile(String content);
		void writeLine(String theLine);
		unsigned long getFilePointerOffset();
		void getFilesize();
		bool IsOpen();
		void Close();
		bool Exists();
		static String getAccessTypeString(AccessType type);

		static List<File *> LoadedFiles;
	};
};