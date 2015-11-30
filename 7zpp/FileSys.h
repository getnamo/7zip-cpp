#pragma once


#include <vector>
#include "FileInfo.h"


namespace SevenZip
{
namespace intl
{
	class FileSys
	{
	public:

		static TString GetPath( const TString& filePath );
		static TString GetFileName( const TString& filePathOrName );
		static TString AppendPath( const TString& left, const TString& right );
		static TString ExtractRelativePath( const TString& basePath, const TString& fullPath );

		static bool DirectoryExists( const TString& path );
		static bool IsDirectoryEmptyRecursive( const TString& path );

		static bool CreateDirectoryTree( const TString& path );

		static std::vector< FilePathInfo > GetFile( const TString& filePathOrName );
		static std::vector< FilePathInfo > GetFilesInDirectory( const TString& directory, const TString& searchPattern, bool recursive );

		static CComPtr< IStream > OpenFileToRead( const TString& filePath );
		static CComPtr< IStream > OpenFileToWrite( const TString& filePath );
	};
}
}
