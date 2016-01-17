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

	SevenZipLister::SevenZipLister(const SevenZipLibrary& library, const TString& archivePath)
		: SevenZipArchive(library, archivePath),
		m_archivePath(archivePath)
	{
	}

	SevenZipLister::~SevenZipLister()
	{
	}

	bool SevenZipLister::ListArchive(ListCallback* callback, const TString& password)
	{
		CComPtr< IStream > fileStream = FileSys::OpenFileToRead(m_archivePath);
		if (fileStream == NULL)
		{
			return false;
			//throw SevenZipException( StrFmt( _T( "Could not open archive \"%s\"" ), m_archivePath.c_str() ) );
		}

		return ListArchive(fileStream, callback, password);
	}

	bool SevenZipLister::ListArchive(const CComPtr< IStream >& archiveStream, ListCallback* callback, const TString& password)
	{
		CComPtr< IInArchive > archive = UsefulFunctions::GetArchiveReader(m_library, m_compressionFormat);
		CComPtr< InStreamWrapper > inFile = new InStreamWrapper(archiveStream);
		CComPtr< ArchiveOpenCallback > openCallback = new ArchiveOpenCallback(password);

		HRESULT hr = archive->Open(inFile, 0, openCallback);
		if (hr != S_OK)
		{
			return false;
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

				ULONGLONG size = prop.uhVal.QuadPart;

				// Get name of file
				archive->GetProperty(i, kpidPath, &prop);

				//valid string? pass back the found value and call the callback function if set
				if (prop.vt == VT_BSTR) {
					WCHAR* path = prop.bstrVal;
					if (callback) {
						callback->OnFileFound(TString(path), size);
					}
				}
			}
		}
		CPropVariant prop;
		archive->Close();

		if (callback) {
			callback->OnListingDone(m_archivePath);
		}
		return true;
	}
}