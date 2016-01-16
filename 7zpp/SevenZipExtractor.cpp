#include "stdafx.h"
#include "SevenZipExtractor.h"
#include "GUIDs.h"
#include "FileSys.h"
#include "ArchiveOpenCallback.h"
#include "ArchiveExtractCallback.h"
#include "InStreamWrapper.h"
#include "PropVariant.h"
#include "UsefulFunctions.h"


namespace SevenZip
{

	using namespace intl;

	SevenZipExtractor::SevenZipExtractor( const SevenZipLibrary& library, const TString& archivePath )
		: SevenZipArchive(library, archivePath)
	{
	}

	SevenZipExtractor::~SevenZipExtractor()
	{
	}

	bool SevenZipExtractor::ExtractArchive( const TString& destDirectory, ProgressCallback* callback )
	{
		CComPtr< IStream > fileStream = FileSys::OpenFileToRead( m_archivePath );

		if ( fileStream == NULL )
		{
			return false;
			//throw SevenZipException( StrFmt( _T( "Could not open archive \"%s\"" ), m_archivePath.c_str() ) );
		}

		return ExtractArchive( fileStream, destDirectory, callback);
	}

	bool SevenZipExtractor::ExtractArchive( const CComPtr< IStream >& archiveStream, const TString& destDirectory, ProgressCallback* callback)
	{
		CComPtr< IInArchive > archive = UsefulFunctions::GetArchiveReader( m_library, m_compressionFormat );
		CComPtr< InStreamWrapper > inFile = new InStreamWrapper( archiveStream );
		CComPtr< ArchiveOpenCallback > openCallback = new ArchiveOpenCallback();

		HRESULT hr = archive->Open( inFile, 0, openCallback );
		if ( hr != S_OK )
		{
			return false;
			//throw SevenZipException( GetCOMErrMsg( _T( "Open archive" ), hr ) );
		}

		CComPtr< ArchiveExtractCallback > extractCallback = new ArchiveExtractCallback( archive, destDirectory, callback );

		hr = archive->Extract( NULL, -1, false, extractCallback );
		if ( hr != S_OK ) // returning S_FALSE also indicates error
		{
			return false;
			//throw SevenZipException( GetCOMErrMsg( _T( "Extract archive" ), hr ) );
		}

		CPropVariant prop;
		archive->GetArchiveProperty(kpidPath, &prop);
		archive->Close();

		if (prop.vt == VT_BSTR) {
			WCHAR* path = prop.bstrVal;
			if (callback)
			{
				callback->OnDone(path);
			}
		}
		return true;
	}

}
