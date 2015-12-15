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

		virtual bool ListArchive( const TString& directory, ListCallback* callback);

	private:
	};
}
