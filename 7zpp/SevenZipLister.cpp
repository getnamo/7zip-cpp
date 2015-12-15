#include "stdafx.h"
#include "SevenZipLister.h"
#include "GUIDs.h"
#include "FileSys.h"
#include "ArchiveOpenCallback.h"
#include "ArchiveExtractCallback.h"
#include "InStreamWrapper.h"


namespace SevenZip
{

	using namespace intl;

	CComPtr< IInArchive > GetArchiveReader(const SevenZipLibrary& library, const CompressionFormatEnum& format)
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

		CComPtr< IInArchive > archive;
		library.CreateObject(*guid, IID_IInArchive, reinterpret_cast<void**>(&archive));
		return archive;
	}

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

	bool SevenZipLister::ListArchive(const TString& directory, ListCallback* callback)
	{

		CComPtr< IInArchive > archive = GetArchiveReader(m_library, m_format);
		// List command
		UInt32 numItems = 0;
		archive->GetNumberOfItems(&numItems);
		for (UInt32 i = 0; i < numItems; i++)
		{
			{
				// Get uncompressed size of file
				PROPVARIANT prop;
				archive->GetProperty(i, kpidSize, &prop);

				int size = prop.intVal;

				// Get name of file
				archive->GetProperty(i, kpidPath, &prop);

				//valid string? pass back the found value and call the callback function if set
				if (prop.vt == VT_BSTR) {
					WCHAR* path = prop.bstrVal;
					if (callback) {
						callback->FileFound(path, size);
					}
				}
			}
		}
		return true;
	}
}