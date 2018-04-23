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

	bool SevenZipLister::ListArchive(const TString& password, ListCallback* callback /*= nullptr*/)
	{
		CComPtr< IStream > fileStream = FileSys::OpenFileToRead(m_archivePath);
		if (fileStream == NULL)
		{
			return false;
			//throw SevenZipException( StrFmt( _T( "Could not open archive \"%s\"" ), m_archivePath.c_str() ) );
		}

		return ListArchive(fileStream, password, callback);
	}

	bool SevenZipLister::ListArchive(const CComPtr< IStream >& archiveStream, const TString& password, ListCallback* callback)
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
			FileInfo info;

			// Get uncompressed size of file
			CPropVariant prop;
			archive->GetProperty(i, kpidSize, &prop);
			info.Size = prop.uhVal.QuadPart;

			// Get packed size of file
			archive->GetProperty(i, kpidPackSize, &prop);
			info.PackedSize = prop.uhVal.QuadPart;

			// Get is directory property
			archive->GetProperty(i, kpidIsDir, &prop);
			info.IsDirectory = prop.boolVal != VARIANT_FALSE;

			// Get name of file
			archive->GetProperty(i, kpidPath, &prop);

			//valid string?
			if (prop.vt == VT_BSTR)
			{
				info.FileName = BstrToTString(prop.bstrVal);
			}

			// Get create time
			archive->GetProperty(i, kpidCTime, &prop);
			info.CreationTime = prop.filetime;

			// Get last access time
			archive->GetProperty(i, kpidATime, &prop);
			info.LastAccessTime = prop.filetime;

			// Get modify time
			archive->GetProperty(i, kpidMTime, &prop);
			info.LastWriteTime = prop.filetime;

			// Get attributes
			archive->GetProperty(i, kpidAttrib, &prop);
			info.Attributes = prop.uintVal;

			// pass back the found value and call the callback function if set
			if (callback)
			{
				callback->OnFileFound(info);
			}
		}

		archive->Close();

		if (callback)
		{
			callback->OnListingDone(m_archivePath);
		}

		return true;
	}
}