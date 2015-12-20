// 7zpp-TestApp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

//  Wrapper
#include "../7zpp/7zpp.h"

//
// 7z.dll directly
//
//
//#include "../7z/CPP/Common/MyWindows.h"
//#include "../7z/CPP/Common/Defs.h"
//#include "../7z/CPP/Common/MyInitGuid.h"
//#include "../7z/CPP/Common/IntToString.h"
//#include "../7z/CPP/Common/StringConvert.h"
//#include "../7z/CPP/Windows/DLL.h"
//#include "../7z/CPP/Windows/FileDir.h"
//#include "../7z/CPP/Windows/FileFind.h"
//#include "../7z/CPP/Windows/FileName.h"
//#include "../7z/CPP/Windows/NtCheck.h"
//#include "../7z/CPP/Windows/PropVariant.h"
//#include "../7z/CPP/Windows/PropVariantConv.h"
//#include "../7z/CPP/7zip/Common/FileStreams.h"
//#include "../7z/CPP/7zip/Archive/IArchive.h"
//#include "../7z/CPP/7zip/IPassword.h"
//#include "../7z/C/7zVersion.h"
//
//DEFINE_GUID(CLSID_CFormat7z,
//	0x23170F69, 0x40C1, 0x278A, 0x10, 0x00, 0x00, 0x01, 0x10, 0x07, 0x00, 0x00);
//DEFINE_GUID(CLSID_CFormatXz,
//	0x23170F69, 0x40C1, 0x278A, 0x10, 0x00, 0x00, 0x01, 0x10, 0x0C, 0x00, 0x00);
//
//#define CLSID_Format CLSID_CFormat7z
//// #define CLSID_Format CLSID_CFormatXz


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

		// Try to detect compression format
		SevenZip::CompressionFormatEnum myCompressionFormat;
		result = extractor.DetectCompressionFormat(myCompressionFormat);

		if (!result)
		{
			std::wcerr
				<< "Could not detect the compression format!"
				<< std::endl;
		}
		else
		{
			std::wcout
				<< "Detected compression: "
				<< myCompressionFormat.GetValue()
				<< std::endl
				<< std::endl;
		}

		extractor.SetCompressionFormat(SevenZip::CompressionFormat::Zip);
		result = extractor.ExtractArchive(myDest, nullptr);

		if (!result)
		{
			std::wcerr
				<< "Could not extract the files!"
				<< std::endl;
		}

		//
		// List
		//
		ListCallBackOutput myListCallBack;

		SevenZip::SevenZipLister lister(lib, myArchive);
		lister.SetCompressionFormat(SevenZip::CompressionFormat::Zip);
		result = lister.ListArchive((SevenZip::ListCallback *)&myListCallBack);

		if (!result)
		{
			std::wcerr
				<< "Could not list the files!"
				<< std::endl;
		}
	}
	else
	{
		std::wcerr
			<< "Could not load the 7Zip DLL!"
			<< std::endl;
	}

	return 0;
}

