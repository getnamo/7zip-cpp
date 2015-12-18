// 7zpp-TestApp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "../7zpp/7zpp.h"

class ListCallBackOutput : SevenZip::ListCallback
{
	virtual void OnFileFound(WCHAR* path, int size)
	{
		std::wcout
			<< path
			<< L" "
			<< size
			<< std::endl;
	}
};


int main()
{

	SevenZip::SevenZipLibrary lib;
	lib.Load();

	SevenZip::TString myArchive(L"ReadMe.zip");

	SevenZip::TString myDest(L"temp");
	SevenZip::SevenZipExtractor extractor(lib, myArchive);
	extractor.SetCompressionFormat(SevenZip::CompressionFormat::Zip);
	extractor.ExtractArchive(myDest, nullptr);

	ListCallBackOutput myListCallBack;

	SevenZip::TString myDir(L"");

	SevenZip::SevenZipLister lister(lib, myArchive);
	lister.SetCompressionFormat(SevenZip::CompressionFormat::Zip);
	lister.ListArchive(myDir, (SevenZip::ListCallback *)&myListCallBack);

	return 0;
}

