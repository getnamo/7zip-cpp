#include "stdafx.h"
#include "SevenZipLibrary.h"
#include "GUIDs.h"


namespace SevenZip
{

const TString DefaultLibraryPath = _T( "7z.dll" );

SevenZipLibrary::SevenZipLibrary()
	: m_dll( NULL )
	, m_func( NULL )
{
}

SevenZipLibrary::~SevenZipLibrary()
{
	Free();
}

bool SevenZipLibrary::Load()
{
	return Load( DefaultLibraryPath );
}

bool SevenZipLibrary::Load( const TString& libraryPath )
{
	Free();
	m_dll = LoadLibrary( libraryPath.c_str() );
	if ( m_dll == NULL )
	{
		return false;
		//throw SevenZipException( GetWinErrMsg( _T( "LoadLibrary" ), GetLastError() ) );
	}

	m_func = reinterpret_cast< CreateObjectFunc >( GetProcAddress( m_dll, "CreateObject" ) );
	if ( m_func == NULL )
	{
		Free();
		return false;
		//throw SevenZipException( _T( "Loaded library is missing required CreateObject function" ) );
	}
	return true;
}

void SevenZipLibrary::Free()
{
	if ( m_dll != NULL )
	{
		FreeLibrary( m_dll );
		m_dll = NULL;
		m_func = NULL;
	}
}

bool SevenZipLibrary::CreateObject( const GUID& clsID, const GUID& interfaceID, void** outObject ) const
{
	if ( m_func == NULL )
	{
		return false;
		//throw SevenZipException( _T( "Library is not loaded" ) );
	}

	HRESULT hr = m_func( &clsID, &interfaceID, outObject );
	if ( FAILED( hr ) )
	{
		return false;
		//throw SevenZipException( GetCOMErrMsg( _T( "CreateObject" ), hr ) );
	}
	return true;
}

CComPtr< IInArchive > UsefulFunctions::GetArchiveReader(const SevenZipLibrary& library, const CompressionFormatEnum& format)
{
	const GUID* guid = NULL;

	switch (format)
	{
	case CompressionFormat::Zip:
		guid = &SevenZip::intl::CLSID_CFormatZip;
		break;

	case CompressionFormat::GZip:
		guid = &SevenZip::intl::CLSID_CFormatGZip;
		break;

	case CompressionFormat::BZip2:
		guid = &SevenZip::intl::CLSID_CFormatBZip2;
		break;

	case CompressionFormat::Rar:
		guid = &SevenZip::intl::CLSID_CFormatRar;
		break;

	case CompressionFormat::Tar:
		guid = &SevenZip::intl::CLSID_CFormatTar;
		break;

	case CompressionFormat::Iso:
		guid = &SevenZip::intl::CLSID_CFormatIso;
		break;

	case CompressionFormat::Cab:
		guid = &SevenZip::intl::CLSID_CFormatCab;
		break;

	case CompressionFormat::Lzma:
		guid = &SevenZip::intl::CLSID_CFormatLzma;
		break;

	case CompressionFormat::Lzma86:
		guid = &SevenZip::intl::CLSID_CFormatLzma86;
		break;

	default:
		guid = &SevenZip::intl::CLSID_CFormat7z;
		break;
	}

	CComPtr< IInArchive > archive;
	library.CreateObject(*guid, IID_IInArchive, reinterpret_cast<void**>(&archive));
	return archive;
}


}
