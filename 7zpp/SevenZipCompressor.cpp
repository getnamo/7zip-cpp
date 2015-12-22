#include "stdafx.h"
#include "SevenZipCompressor.h"
#include "GUIDs.h"
#include "FileSys.h"
#include "ArchiveUpdateCallback.h"
#include "OutStreamWrapper.h"
#include "PropVariant.h"


namespace SevenZip
{

using namespace intl;


const TString SearchPatternAllFiles = _T( "*" );

CComPtr< IOutArchive > GetArchiveWriter(const SevenZipLibrary& library, const CompressionFormatEnum& format)
{
   const GUID* guid = NULL;

   switch (format)
   {
   case CompressionFormat::Zip:
      guid = &CLSID_CFormatZip;
      break;

   case CompressionFormat::GZip:
      guid = &CLSID_CFormatGZip;
      break;

   case CompressionFormat::BZip2:
      guid = &CLSID_CFormatBZip2;
      break;

   case CompressionFormat::Rar:
      guid = &CLSID_CFormatRar;
      break;

   case CompressionFormat::Tar:
      guid = &CLSID_CFormatTar;
      break;

   case CompressionFormat::Iso:
      guid = &CLSID_CFormatIso;
      break;

   case CompressionFormat::Cab:
      guid = &CLSID_CFormatCab;
      break;

   case CompressionFormat::Lzma:
      guid = &CLSID_CFormatLzma;
      break;

   case CompressionFormat::Lzma86:
      guid = &CLSID_CFormatLzma86;
      break;

   default:
      guid = &CLSID_CFormat7z;
      break;
   }

   CComPtr< IOutArchive > archive;
   library.CreateObject(*guid, IID_IOutArchive, reinterpret_cast< void** >(&archive));
   return archive;
}

SevenZipCompressor::SevenZipCompressor( const SevenZipLibrary& library, const TString& archivePath )
	: m_library( library )
	, m_archivePath( archivePath )
{
}

SevenZipCompressor::~SevenZipCompressor()
{
}

void SevenZipCompressor::SetCompressionFormat(const CompressionFormatEnum& format)
{
   m_compressionFormat = format;
}

void SevenZipCompressor::SetCompressionLevel( const CompressionLevelEnum& level )
{
	m_compressionLevel = level;
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
		return false;
		//throw SevenZipException( StrFmt( _T( "Directory \"%s\" does not exist" ), directory.c_str() ) );
	}
	
	if ( FileSys::IsDirectoryEmptyRecursive( directory ) )
	{
		return false;
		//throw SevenZipException( StrFmt( _T( "Directory \"%s\" is empty" ), directory.c_str() ) );
	}

	std::vector< FilePathInfo > files = FileSys::GetFilesInDirectory( directory, searchPattern, recursion );
	return CompressFilesToArchive( pathPrefix, files, callback );
}

bool SevenZipCompressor::CompressFilesToArchive(const TString& pathPrefix, const std::vector< FilePathInfo >& filePaths,
	ProgressCallback* progressCallback)
{
   CComPtr< IOutArchive > archiver = GetArchiveWriter(m_library, m_compressionFormat);

	SetCompressionProperties( archiver );

	CComPtr< OutStreamWrapper > outFile = new OutStreamWrapper( OpenArchiveStream() );
	CComPtr< ArchiveUpdateCallback > updateCallback = new ArchiveUpdateCallback( pathPrefix, filePaths, progressCallback );

	HRESULT hr = archiver->UpdateItems( outFile, (UInt32) filePaths.size(), updateCallback );

	if (progressCallback)
	{
		progressCallback->OnDone();
	}

	if ( hr != S_OK ) // returning S_FALSE also indicates error
	{
		return false;
		//throw SevenZipException( GetCOMErrMsg( _T( "Create archive" ), hr ) );
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
		return false;
		//throw SevenZipException( _T( "Archive does not support setting compression properties" ) );
	}

	HRESULT hr = setter->SetProperties( names, values, numProps );
	if ( hr != S_OK )
	{
		return false;
		//throw SevenZipException( GetCOMErrMsg( _T( "Setting compression properties" ), hr ) );
	}
	return true;
}

}
