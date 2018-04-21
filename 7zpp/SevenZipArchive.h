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

		virtual bool ReadInArchiveMetadata();

		virtual void SetCompressionFormat(const CompressionFormatEnum& format);
		virtual CompressionFormatEnum GetCompressionFormat();

		virtual void SetCompressionLevel(const CompressionLevelEnum& level);
		virtual CompressionLevelEnum GetCompressionLevel();

		virtual bool DetectCompressionFormat();

		virtual size_t GetNumberOfItems();
		virtual std::vector<std::wstring> GetItemsNames();
		virtual std::vector<size_t>  GetOrigSizes();
		virtual void SetPassword(const TString& password) { m_password = password; }

	protected:
		bool m_ReadMetadata = false;
		bool m_OverrideCompressionFormat = false;
		const SevenZipLibrary& m_library;
		TString m_archivePath;
		CompressionFormatEnum m_compressionFormat;
		CompressionLevelEnum m_compressionLevel;
		size_t m_numberofitems = 0;
		std::vector<std::wstring> m_itemnames;
		std::vector<size_t> m_origsizes;
		TString m_password;

	private:
		bool pri_GetNumberOfItems();
		bool pri_GetItemsNames();
		bool pri_DetectCompressionFormat(CompressionFormatEnum & format);
		bool pri_DetectCompressionFormat();
	};
}
