#include "stdafx.h"
#include "UsefulFunctions.h"
#include <vector>

namespace SevenZip
{
	using namespace intl;

	CComPtr< IInArchive > UsefulFunctions::GetArchiveReader(const SevenZipLibrary& library, const CompressionFormatEnum& format)
	{
		const GUID* guid = GetCompressionGUID(format);

		CComPtr< IInArchive > archive;
		library.CreateObject(*guid, IID_IInArchive, reinterpret_cast<void**>(&archive));
		return archive;
	}

	bool UsefulFunctions::DetectCompressionFormat(const SevenZipLibrary & library, const TString & archivePath, 
		CompressionFormatEnum & archiveCompressionFormat)
	{
		CComPtr< IStream > fileStream = FileSys::OpenFileToRead(archivePath);

		if (fileStream == NULL)
		{
			return false;
			//throw SevenZipException( StrFmt( _T( "Could not open archive \"%s\"" ), m_archivePath.c_str() ) );
		}

		std::vector<CompressionFormatEnum> myAvailableFormats;

		// Add more formats here if 7zip supports more formats in the future
		myAvailableFormats.push_back(CompressionFormat::Zip);
		myAvailableFormats.push_back(CompressionFormat::SevenZip);
		myAvailableFormats.push_back(CompressionFormat::Rar);
		myAvailableFormats.push_back(CompressionFormat::GZip);
		myAvailableFormats.push_back(CompressionFormat::BZip2);
		myAvailableFormats.push_back(CompressionFormat::Tar);
		myAvailableFormats.push_back(CompressionFormat::Lzma);
		myAvailableFormats.push_back(CompressionFormat::Lzma86);
		myAvailableFormats.push_back(CompressionFormat::Cab);
		myAvailableFormats.push_back(CompressionFormat::Iso);

		// Check each format for one that works
		for (int i = 0; i < myAvailableFormats.size(); i++)
		{
			archiveCompressionFormat = myAvailableFormats[i];

			CComPtr< IInArchive > archive = UsefulFunctions::GetArchiveReader(library, archiveCompressionFormat);
			CComPtr< InStreamWrapper > inFile = new InStreamWrapper(fileStream);
			CComPtr< ArchiveOpenCallback > openCallback = new ArchiveOpenCallback();

			HRESULT hr = archive->Open(inFile, 0, openCallback);
			if (hr == S_OK)
			{
				// We know the format if we get here, so return
				archive->Close();
				return true;
				//throw SevenZipException( GetCOMErrMsg( _T( "Open archive" ), hr ) );
			}

			archive->Close();
		}

		// If you get here, the format is unknown
		archiveCompressionFormat = CompressionFormat::Unknown;
		return true;
	}

	const GUID* UsefulFunctions::GetCompressionGUID(const CompressionFormatEnum& format)
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
		return guid;
	}

}

