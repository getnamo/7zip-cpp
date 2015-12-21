#pragma once

#include "SevenZipLibrary.h"
#include <atlbase.h>
#include "FileInfo.h"
#include "CompressionFormat.h"
#include "CompressionLevel.h"

namespace SevenZip
{
	class SevenZipArchive
	{
	public:
		SevenZipArchive(const SevenZipLibrary& library, const TString& archivePath);
		virtual ~SevenZipArchive();

		virtual void SetCompressionFormat(const CompressionFormatEnum& format);
		virtual CompressionFormatEnum GetCompressionFormat();

		virtual size_t GetNumberOfItems();
		virtual std::vector<std::wstring> GetItemsNames();
		virtual std::vector<size_t>  GetOrigSizes();

	protected:
		const SevenZipLibrary& m_library;
		TString m_archivePath;
		CompressionFormatEnum m_compressionFormat;
		size_t m_numberofitems = 0;
		std::vector<std::wstring> m_itemnames;
		std::vector<size_t> m_origsizes;

	private:
		bool pri_GetNumberOfItems();
		bool pri_GetItemsNames();
		bool pri_DetectCompressionFormat(CompressionFormatEnum & format);
		bool pri_DetectCompressionFormat();
	};
}
