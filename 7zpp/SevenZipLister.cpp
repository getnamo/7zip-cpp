#include "stdafx.h"
#include "SevenZipLister.h"
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

	void SevenZipLister::SetCompressionFormat(const CompressionFormatEnum& format)
	{
		m_format = format;
	}

	SevenZipLister::SevenZipLister(const SevenZipLibrary& library, const TString& archivePath)
		: m_library(library)
		, m_archivePath(archivePath)
		, m_format(CompressionFormat::SevenZip)
	{

	}

	SevenZipLister::~SevenZipLister()
	{

	}

	bool SevenZipLister::DetectCompressionFormat()
	{
		return DetectCompressionFormat(m_format);
	}

	bool SevenZipLister::DetectCompressionFormat(CompressionFormatEnum & format)
	{
		return UsefulFunctions::DetectCompressionFormat(m_library, m_archivePath, format);
	}

	bool SevenZipLister::ListArchive(ListCallback* callback)
	{
		CComPtr< IStream > fileStream = FileSys::OpenFileToRead(m_archivePath);
		if (fileStream == NULL)
		{
			return false;
			//throw SevenZipException( StrFmt( _T( "Could not open archive \"%s\"" ), m_archivePath.c_str() ) );
		}

		return ListArchive(fileStream, callback);
	}

	bool SevenZipLister::ListArchive(const CComPtr< IStream >& archiveStream, ListCallback* callback)
	{
		CComPtr< IInArchive > archive = UsefulFunctions::GetArchiveReader(m_library, m_format);
		CComPtr< InStreamWrapper > inFile = new InStreamWrapper(archiveStream);
		CComPtr< ArchiveOpenCallback > openCallback = new ArchiveOpenCallback();

		HRESULT hr = archive->Open(inFile, 0, openCallback);
		if (hr != S_OK)
		{
			return false;
			//throw SevenZipException( GetCOMErrMsg( _T( "Open archive" ), hr ) );
		}

		// List command
		UInt32 numItems = 0;
		archive->GetNumberOfItems(&numItems);
		for (UInt32 i = 0; i < numItems; i++)
		{
			{
				// Get uncompressed size of file
				CPropVariant prop;
				archive->GetProperty(i, kpidSize, &prop);

				int size = prop.intVal;

				// Get name of file
				archive->GetProperty(i, kpidPath, &prop);

				//valid string? pass back the found value and call the callback function if set
				if (prop.vt == VT_BSTR) {
					WCHAR* path = prop.bstrVal;
					if (callback) {
						callback->OnFileFound(path, size);
					}
				}
			}
		}
		archive->Close();
		return true;
	}
}