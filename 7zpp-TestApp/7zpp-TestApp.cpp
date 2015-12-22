// 7zpp-TestApp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

//  Wrapper
#include "../7zpp/7zpp.h"

//
// For wrappers...
//
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
	//
	// Using the wrappers...
	//

	SevenZip::SevenZipLibrary lib;
	bool result = lib.Load(SevenZip::TString(L"\\source\\7zip-cpp\\Exe\\x64\\7z_dist.dll"));
	if (result)
	{
		SevenZip::TString myArchive(L"\\source\\7zip-cpp\\exe\\x64\\files.zip");

		wchar_t currdir[2000];
		GetCurrentDirectory(2000, currdir);

		SevenZip::TString myDest(L"\\source\\7zip-cpp\\exe\\x64\\temp");

		// 
		// Extract
		//
		SevenZip::SevenZipExtractor extractor(lib, myArchive);

		//
		// Try to detect compression format, num of items, and names
		//
		SevenZip::CompressionFormatEnum myCompressionFormat;

		// Read in all the metadata
		extractor.ReadInArchiveMetadata();

		// Pull the metadata locally
		myCompressionFormat = extractor.GetCompressionFormat();
		size_t numberofitems = extractor.GetNumberOfItems();
		std::vector<std::wstring> itemnames = extractor.GetItemsNames();
		std::vector<size_t> origsizes = extractor.GetOrigSizes();

		std::wcout
			<< L"Detected compression: "
			<< myCompressionFormat.GetValue()
			<< std::endl
			<< std::endl;

		std::wcout
			<< L"Number of Items: "
			<< numberofitems
			<< std::endl
			<< std::endl;

		std::wcout
			<< L"Names of Items: "
			<< std::endl
			<< std::endl;

		for (int i = 0; i < itemnames.size(); i++)
		{
			std::wcout
				<< i+1
				<< L": "
				<< itemnames[i]
				<< L" "
				<< origsizes[i]
				<< std::endl;
		}

		std::wcout
			<< std::endl
			<< std::endl;

		//
		//  Using callbacks
		//
		extractor.SetCompressionFormat(SevenZip::CompressionFormat::Zip);
		result = extractor.ExtractArchive(myDest, nullptr);

		if (!result)
		{
			std::wcerr
				<< L"Could not extract the files!"
				<< std::endl;
		}

		//
		// Lister
		//
		ListCallBackOutput myListCallBack;

		std::wcout
			<< L"List using callbacks: "
			<< std::endl
			<< std::endl;

		SevenZip::SevenZipLister lister(lib, myArchive);
		lister.SetCompressionFormat(SevenZip::CompressionFormat::Zip);
		result = lister.ListArchive((SevenZip::ListCallback *)&myListCallBack);

		if (!result)
		{
			std::wcerr
				<< L"Could not list the files!"
				<< std::endl;
		}
	}
	else
	{
		std::wcerr
			<< L"Could not load the 7Zip DLL!"
			<< std::endl;
	}

	return 0;
}

