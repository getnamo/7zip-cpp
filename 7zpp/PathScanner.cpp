#include "StdAfx.h"
#include "PathScanner.h"
#include "FileSys.h"


namespace SevenZip
{
namespace intl
{

void PathScanner::Scan( const TString& root, const TString& pathPrefix, Callback& cb )
{
	Scan(root, pathPrefix, _T("*"), cb);
}

void PathScanner::Scan( const TString& root, const TString& pathPrefix, const TString& searchPattern, Callback& cb )
{
	std::deque< TString > directories;
	directories.push_back( root );

	while ( !directories.empty() )
	{
		TString directory = directories.front();
		directories.pop_front();

		if ( ExamineFiles( directory, searchPattern, pathPrefix, cb ) )
		{
			break;
		}

		ExamineDirectories(directory, directories, pathPrefix, cb);
	}
}

bool PathScanner::ExamineFiles( const TString& directory, const TString& searchPattern, const TString& pathPrefix, Callback& cb )
{
	TString findStr = FileSys::AppendPath( directory, searchPattern );
	bool exit = false;

	WIN32_FIND_DATA fdata;
	HANDLE hFile = FindFirstFile( findStr.c_str(), &fdata );
	if ( hFile == INVALID_HANDLE_VALUE )
	{
		return exit;
	}

	cb.EnterDirectory( directory );

	do
	{
		FilePathInfo fpInfo = ConvertFindInfo( directory, pathPrefix, fdata );
		if ( !fpInfo.IsDirectory && !IsSpecialFileName( fpInfo.FileName ) )
		{
			cb.ExamineFile( fpInfo, exit );
		}
	}
	while ( !exit && FindNextFile( hFile, &fdata ) );

	if ( !exit )
	{
		cb.LeaveDirectory( directory );
	}

	FindClose( hFile );
	return exit;
}

void PathScanner::ExamineDirectories( const TString& directory, std::deque< TString >& subDirs, const TString& pathPrefix, Callback& cb )
{
	TString findStr = FileSys::AppendPath( directory, _T( "*" ) );

	WIN32_FIND_DATA fdata;
	HANDLE hFile = FindFirstFile( findStr.c_str(), &fdata );
	if ( hFile == INVALID_HANDLE_VALUE )
	{
		return;
	}

	do
	{
		FilePathInfo fpInfo = ConvertFindInfo( directory, pathPrefix, fdata );
		if ( fpInfo.IsDirectory && !IsSpecialFileName( fpInfo.FileName ) && cb.ShouldDescend( fpInfo ) )
		{
			subDirs.push_back( fpInfo.FilePath );
		}
	}
	while ( FindNextFile( hFile, &fdata ) );

	FindClose( hFile );
}

bool PathScanner::IsAllFilesPattern( const TString& searchPattern )
{
	return searchPattern == _T( "*" ) || searchPattern == _T( "*.*" );
}

bool PathScanner::IsSpecialFileName( const TString& fileName )
{
	return fileName == _T( "." ) || fileName == _T( ".." );
}

bool PathScanner::IsDirectory( const WIN32_FIND_DATA& fdata )
{
	return ( fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) != 0;
}

FilePathInfo PathScanner::ConvertFindInfo( const TString& directory, const TString& pathPrefix, const WIN32_FIND_DATA& fdata )
{
	FilePathInfo file;
	file.memFile		= false;
	file.rootPath		= pathPrefix;
	file.FileName		= fdata.cFileName;
	file.FilePath		= FileSys::AppendPath( directory, file.FileName );
	file.LastWriteTime	= fdata.ftLastWriteTime;
	file.CreationTime	= fdata.ftCreationTime;
	file.LastAccessTime	= fdata.ftLastAccessTime;
	file.Attributes		= fdata.dwFileAttributes;
	file.IsDirectory	= IsDirectory( fdata );

	ULARGE_INTEGER size;
	size.LowPart = fdata.nFileSizeLow;
	size.HighPart = fdata.nFileSizeHigh;
	file.Size = size.QuadPart;

	return file;
}

}
}
