#pragma once

#include "SevenZipLibrary.h"
#include "SevenZipArchive.h"
#include "CompressionFormat.h"
#include "ArchiveListCallback.h"


namespace SevenZip
{
	class SevenZipLister : public SevenZipArchive
	{
	public:
		TString m_archivePath;

		SevenZipLister( const SevenZipLibrary& library, const TString& archivePath );
		virtual ~SevenZipLister();

		virtual bool ListArchive(const TString& password, ArchiveListCallback* callback = nullptr);

	private:
		bool ListArchive(const CComPtr< IStream >& archiveStream, const TString& password, ArchiveListCallback* callback);
	};
}
