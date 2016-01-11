#include "stdafx.h"
#include "SevenZipArchive.h"
#include "UsefulFunctions.h"


namespace SevenZip
{
	SevenZipArchive::SevenZipArchive(const SevenZipLibrary& library, const TString& archivePath)
		: m_library(library),
		m_archivePath(archivePath),
		// The default compression type will be unknown
		m_compressionFormat(CompressionFormat::Unknown),
		m_compressionLevel(CompressionLevel::None)
	{
	}

	SevenZipArchive::~SevenZipArchive()
	{
		m_itemnames.clear();
		m_origsizes.clear();
	}

	void SevenZipArchive::SetCompressionFormat(const CompressionFormatEnum& format)
	{
		m_OverrideCompressionFormat = true;
		m_ReadMetadata = false;
		m_compressionFormat = format;
	}

	void SevenZipArchive::SetCompressionLevel(const CompressionLevelEnum& level)
	{
		m_compressionLevel = level;
	}

	CompressionLevelEnum SevenZipArchive::GetCompressionLevel()
	{
		return m_compressionLevel;
	}

	CompressionFormatEnum SevenZipArchive::GetCompressionFormat()
	{
		if (!m_ReadMetadata && !m_OverrideCompressionFormat)
		{
			m_ReadMetadata = ReadInArchiveMetadata();
		}
		return m_compressionFormat;
	}

	size_t SevenZipArchive::GetNumberOfItems()
	{
		if (!m_ReadMetadata)
		{
			m_ReadMetadata = ReadInArchiveMetadata();
		}
		return m_numberofitems;
	}

	std::vector<TString> SevenZipArchive::GetItemsNames()
	{
		if (!m_ReadMetadata)
		{
			m_ReadMetadata = ReadInArchiveMetadata();
		}
		return m_itemnames;
	}

	std::vector<size_t> SevenZipArchive::GetOrigSizes()
	{
		if (!m_ReadMetadata)
		{
			m_ReadMetadata = ReadInArchiveMetadata();
		}
		return m_origsizes;
	}

	// Sets up all the metadata for an archive file
	bool SevenZipArchive::ReadInArchiveMetadata()
	{
		bool DetectedCompressionFormat = true;
		if (!m_OverrideCompressionFormat)
		{
			DetectedCompressionFormat = pri_DetectCompressionFormat();
		}
		bool GotItemNumberNamesAndOrigSizes = pri_GetItemsNames();

		return (DetectedCompressionFormat && GotItemNumberNamesAndOrigSizes);
	}

	bool SevenZipArchive::DetectCompressionFormat()
	{
		m_OverrideCompressionFormat = false;
		return pri_DetectCompressionFormat();
	}

	bool SevenZipArchive::pri_DetectCompressionFormat()
	{
		m_OverrideCompressionFormat = false;
		return pri_DetectCompressionFormat(m_compressionFormat);
	}

	bool SevenZipArchive::pri_DetectCompressionFormat(CompressionFormatEnum & format)
	{
		m_OverrideCompressionFormat = false;
		return UsefulFunctions::DetectCompressionFormat(m_library, m_archivePath, format);
	}

	bool SevenZipArchive::pri_GetNumberOfItems()
	{
		return UsefulFunctions::GetNumberOfItems(m_library, m_archivePath, 
			m_compressionFormat, m_numberofitems);
	}

	bool SevenZipArchive::pri_GetItemsNames()
	{
		return UsefulFunctions::GetItemsNames(m_library, m_archivePath, m_compressionFormat, 
			m_numberofitems, m_itemnames, m_origsizes);
	}
}

