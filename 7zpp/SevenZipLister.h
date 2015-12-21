#pragma once

#include "SevenZipLibrary.h"
#include "CompressionFormat.h"
#include "ListCallback.h"


namespace SevenZip
{
	class SevenZipLister
	{
	private:

		const SevenZipLibrary& m_library;
		TString m_archivePath;
		CompressionFormatEnum m_format;

	public:

		SevenZipLister( const SevenZipLibrary& library, const TString& archivePath );
		virtual ~SevenZipLister();

		void SetCompressionFormat(const CompressionFormatEnum& format);

		bool DetectCompressionFormat(CompressionFormatEnum & format);
		bool DetectCompressionFormat();

		virtual bool ListArchive(ListCallback* callback);

	private:
		bool ListArchive(const CComPtr< IStream >& archiveStream, ListCallback* callback);
	};
}
