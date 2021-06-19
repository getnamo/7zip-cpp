#pragma once


#include "SevenZipLibrary.h"
#include "SevenZipArchive.h"
#include "ProgressCallback.h"


namespace SevenZip
{
	class SevenZipExtractor : public SevenZipArchive
	{
	public:

		SevenZipExtractor( const SevenZipLibrary& library, const TString& archivePath );
		virtual ~SevenZipExtractor();

		virtual bool ExtractArchive( const TString& directory, ProgressCallback* callback = nullptr);
		virtual bool ExtractFilesFromArchive(const unsigned int* fileIndices,
											 const unsigned int numberFiles,
											 const TString& directory,
											 ProgressCallback* callback = nullptr);
		virtual bool ExtractFileToMemory(const unsigned int index, std::vector<BYTE>& out_buffer, ProgressCallback* callback = nullptr);

	private:
		bool ExtractFilesFromArchive(const CComPtr<IStream>& archiveStream,
									 const unsigned int* fileIndices,
									 const unsigned int numberFiles,
									 const TString& directory,
									 ProgressCallback* callback);
	};
}
