#pragma once


namespace SevenZip
{
namespace intl
{
	struct FileInfo
	{
		TString		FileName;
		FILETIME	LastWriteTime;
		FILETIME	CreationTime;
		FILETIME	LastAccessTime;
		ULONGLONG	Size;
		ULONGLONG	PackedSize;
		UINT		Attributes;
		bool		IsDirectory;
		bool		memFile;
		void*		memPointer;
	};

	struct FilePathInfo : public FileInfo
	{
		TString		rootPath;
		TString		FilePath;
	};
}
}
