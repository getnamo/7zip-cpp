#include "stdafx.h"
#include "UsefulFunctions.h"

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

