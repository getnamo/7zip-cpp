#include "stdafx.h"
#include "SevenZipCompressor.h"
#include "GUIDs.h"
#include "FileSys.h"
#include "ArchiveUpdateCallback.h"
#include "OutStreamWrapper.h"
#include "PropVariant.h"
#include "UsefulFunctions.h"


namespace SevenZip
{

using namespace intl;


const TString SearchPatternAllFiles = _T( "*" );

SevenZipCompressor::SevenZipCompressor(const SevenZipLibrary& library, const TString& archivePath)
	: SevenZipArchive(library, archivePath)
{
}

SevenZipCompressor::~SevenZipCompressor()
{
}

bool SevenZipCompressor::CompressDirectory( const TString& directory, ProgressCallback* callback, bool includeSubdirs )
{	
	return FindAndCompressFiles( 
			directory, 
			SearchPatternAllFiles, 
			FileSys::GetPath( directory ), 
		includeSubdirs,
		callback);
}

bool SevenZipCompressor::CompressFiles( const TString& directory, const TString& searchFilter, ProgressCallback* callback, bool includeSubdirs )
{
	return FindAndCompressFiles( 
			directory, 
			searchFilter, 
			directory, 
			includeSubdirs,
			callback);
}

bool SevenZipCompressor::CompressAllFiles( const TString& directory, ProgressCallback* callback, bool includeSubdirs )
{
	return FindAndCompressFiles( 
			directory, 
			SearchPatternAllFiles, 
			directory, 
			includeSubdirs,
			callback);
}

bool SevenZipCompressor::CompressFile( const TString& filePath, ProgressCallback* callback)
{
	std::vector< FilePathInfo > files = FileSys::GetFile( filePath );

	if ( files.empty() )
	{
		return false;
		//throw SevenZipException( StrFmt( _T( "File \"%s\" does not exist" ), filePath.c_str() ) );
	}

	m_outputPath = filePath;

	return CompressFilesToArchive( TString(), files, callback );
}

CComPtr< IStream > SevenZipCompressor::OpenArchiveStream()
{
	CComPtr< IStream > fileStream = FileSys::OpenFileToWrite( m_archivePath );
	if ( fileStream == NULL )
	{
		return NULL;
		//throw SevenZipException( StrFmt( _T( "Could not create archive \"%s\"" ), m_archivePath.c_str() ) );
	}
	return fileStream;
}

bool SevenZipCompressor::FindAndCompressFiles( const TString& directory, const TString& searchPattern, const TString& pathPrefix, bool recursion, ProgressCallback* callback )
{
	if ( !FileSys::DirectoryExists( directory ) )
	{
		return false;	//Directory does not exist
	}
	
	if ( FileSys::IsDirectoryEmptyRecursive( directory ) )
	{
		return false;	//Directory \"%s\" is empty" ), directory.c_str()
	}

	m_outputPath = directory;

	std::vector< FilePathInfo > files = FileSys::GetFilesInDirectory( directory, searchPattern, recursion );
	return CompressFilesToArchive( pathPrefix, files, callback );
}

bool SevenZipCompressor::CompressFilesToArchive(const TString& pathPrefix, const std::vector< FilePathInfo >& filePaths,
	ProgressCallback* progressCallback)
{
	CComPtr< IOutArchive > archiver = UsefulFunctions::GetArchiveWriter(m_library, m_compressionFormat);
	SetCompressionProperties( archiver );

	//Set full outputFilePath including ending
	m_outputPath += UsefulFunctions::EndingFromCompressionFormat(m_compressionFormat);

	CComPtr< OutStreamWrapper > outFile = new OutStreamWrapper( OpenArchiveStream() );
	CComPtr< ArchiveUpdateCallback > updateCallback = new ArchiveUpdateCallback( pathPrefix, filePaths, m_outputPath, progressCallback );

	HRESULT hr = archiver->UpdateItems( outFile, (UInt32) filePaths.size(), updateCallback );

	if (progressCallback)
	{
		progressCallback->OnDone(m_outputPath);	//Todo: give full path support
	}

	if ( hr != S_OK ) // returning S_FALSE also indicates error
	{
		return false;
	}
	return true;
}

bool SevenZipCompressor::SetCompressionProperties( IUnknown* outArchive )
{
	const size_t numProps = 1;
	const wchar_t* names[numProps] = { L"x" };
	CPropVariant values[numProps] = { static_cast< UInt32 >( m_compressionLevel.GetValue() ) };

	CComPtr< ISetProperties > setter;
	outArchive->QueryInterface( IID_ISetProperties, reinterpret_cast< void** >( &setter ) );
	if ( setter == NULL )
	{
		return false;	//Archive does not support setting compression properties
	}

	HRESULT hr = setter->SetProperties( names, values, numProps );
	if ( hr != S_OK )
	{
		return false;	//Setting compression properties
	}
	return true;
}

}
