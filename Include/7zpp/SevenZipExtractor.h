#pragma once


#include "SevenZipLibrary.h"
#include "CompressionFormat.h"
#include "ProgressCallback.h"


namespace SevenZip
{
	class SevenZipExtractor
	{
	private:

		const SevenZipLibrary& m_library;
		TString m_archivePath;
		CompressionFormatEnum m_format;

	public:

		SevenZipExtractor( const SevenZipLibrary& library, const TString& archivePath );
		virtual ~SevenZipExtractor();

		void SetCompressionFormat( const CompressionFormatEnum& format );

		virtual bool ExtractArchive( const TString& directory, ProgressCallback* callback);

	private:

		bool ExtractArchive( const CComPtr< IStream >& archiveStream, const TString& directory, ProgressCallback* callback);
	};
}
