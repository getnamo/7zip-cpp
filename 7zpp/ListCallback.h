#pragma once


#include "SevenZipLibrary.h"
#include "CompressionFormat.h"


namespace SevenZip
{
	using namespace intl;

	class ListCallback
	{
	public:
		/*
		Called for each file found in the archive. Size in bytes.
		*/
		virtual void OnFileFound(const FileInfo& fileInfo) {}

		/*
		Called when all the files have been listed
		*/
		virtual void OnListingDone(const TString& path) {}
	};
}
