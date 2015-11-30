#include "stdafx.h"
#include "SevenZipExtractor.h"
#include "GUIDs.h"
#include "FileSys.h"
#include "ArchiveOpenCallback.h"
#include "ArchiveExtractCallback.h"
#include "InStreamWrapper.h"


namespace SevenZip
{

using namespace intl;


CComPtr< IInArchive > GetArchiveReader( const SevenZipLibrary& library, const CompressionFormatEnum& format )
{
	const GUID* guid = NULL;

	switch ( format )
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
	
	CComPtr< IInArchive > archive;
	library.CreateObject( *guid, IID_IInArchive, reinterpret_cast< void** >( &archive ) );
	return archive;
}


SevenZipExtractor::SevenZipExtractor( const SevenZipLibrary& library, const TString& archivePath )
	: m_library( library )
	, m_archivePath( archivePath )
	, m_format( CompressionFormat::SevenZip )
{
}

SevenZipExtractor::~SevenZipExtractor()
{
}

void SevenZipExtractor::SetCompressionFormat( const CompressionFormatEnum& format )
{
	m_format = format;
}

void SevenZipExtractor::ExtractArchive( const TString& destDirectory )
{
	CComPtr< IStream > fileStream = FileSys::OpenFileToRead( m_archivePath );
	if ( fileStream == NULL )
	{
		throw SevenZipException( StrFmt( _T( "Could not open archive \"%s\"" ), m_archivePath.c_str() ) );
	}

	ExtractArchive( fileStream, destDirectory );
}

void SevenZipExtractor::ExtractArchive( const CComPtr< IStream >& archiveStream, const TString& destDirectory )
{
	CComPtr< IInArchive > archive = GetArchiveReader( m_library, m_format );
	CComPtr< InStreamWrapper > inFile = new InStreamWrapper( archiveStream );
	CComPtr< ArchiveOpenCallback > openCallback = new ArchiveOpenCallback();

	HRESULT hr = archive->Open( inFile, 0, openCallback );
	if ( hr != S_OK )
	{
		throw SevenZipException( GetCOMErrMsg( _T( "Open archive" ), hr ) );
	}

	CComPtr< ArchiveExtractCallback > extractCallback = new ArchiveExtractCallback( archive, destDirectory );

	hr = archive->Extract( NULL, -1, false, extractCallback );
	if ( hr != S_OK ) // returning S_FALSE also indicates error
	{
		throw SevenZipException( GetCOMErrMsg( _T( "Extract archive" ), hr ) );
	}
}

}
