#pragma once

#include "SevenZipLibrary.h"
#include "CompressionFormat.h"
#include <7zip/Archive/IArchive.h>
#include <7zTypes.h>
#include "GUIDs.h"
#include "FileSys.h"
#include "ArchiveOpenCallback.h"
#include "InStreamWrapper.h"

namespace SevenZip
{
	using namespace intl;

	class UsefulFunctions
	{
	public:
		static const GUID* GetCompressionGUID(const CompressionFormatEnum& format);

		static CComPtr< IInArchive > GetArchiveReader(const SevenZipLibrary& library, const CompressionFormatEnum& format);
		static CComPtr< IOutArchive > GetArchiveWriter(const SevenZipLibrary& library, const CompressionFormatEnum& format);

		static bool DetectCompressionFormat(const SevenZipLibrary& library, const TString& archivePath,
			CompressionFormatEnum & archiveCompressionFormat, const TString& password);

		static bool UsefulFunctions::GetNumberOfItems(const SevenZipLibrary & library, const TString & archivePath,
			CompressionFormatEnum & format, size_t & numberofitems, const TString& password);

		static bool UsefulFunctions::GetItemsNames(const SevenZipLibrary & library, const TString & archivePath,
			CompressionFormatEnum & format, size_t & numberofitems,
			std::vector<std::wstring> & itemnames, std::vector<size_t> & origsizes, const TString& password);

		static const TString EndingFromCompressionFormat(const CompressionFormatEnum& format);
	};
}
