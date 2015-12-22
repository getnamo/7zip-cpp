#include "stdafx.h"
#include "SevenZipArchive.h"
#include "UsefulFunctions.h"


namespace SevenZip
{
	SevenZipArchive::SevenZipArchive(const SevenZipLibrary& library, const TString& archivePath)
		: m_library(library),
		m_archivePath(archivePath),
		m_compressionFormat(CompressionFormat::SevenZip)
	{
	}

	SevenZipArchive::~SevenZipArchive()
	{
		m_itemnames.clear();
	}

	void SevenZipArchive::SetCompressionFormat(const CompressionFormatEnum& format)
	{
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
		pri_DetectCompressionFormat();
		return m_compressionFormat;
	}

	size_t SevenZipArchive::GetNumberOfItems()
	{
		pri_GetItemsNames();
		return m_numberofitems;
	}

	std::vector<TString> SevenZipArchive::GetItemsNames()
	{
		pri_GetItemsNames();
		return m_itemnames;
	}

	std::vector<size_t> SevenZipArchive::GetOrigSizes()
	{
		pri_GetItemsNames();
		return m_origsizes;
	}

	bool SevenZipArchive::pri_DetectCompressionFormat()
	{
		return pri_DetectCompressionFormat(m_compressionFormat);
	}

	bool SevenZipArchive::pri_DetectCompressionFormat(CompressionFormatEnum & format)
	{
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

