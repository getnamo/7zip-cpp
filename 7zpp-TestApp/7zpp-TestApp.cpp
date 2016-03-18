// 7zpp-TestApp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <gtest/gtest.h>
#include <boost\filesystem.hpp>

//  Wrapper
#include "../7zpp/7zpp.h"

#define DLL_PATH SOLUTIONDIR L"Exe\\x64\\7z.dll"
#define TEMPDIR SOLUTIONDIR L"Exe\\x64\\tmp"
#define TESTEXTRACTTESTFILE1 SOLUTIONDIR L"7zpp-TestApp\\TestFiles\\files.zip"
#define TESTEXTRACTTESTFILE2 SOLUTIONDIR L"7zpp-TestApp\\TestFiles\\Readme.txt.gz"
#define TESTEXTRACTTESTFILE3 SOLUTIONDIR L"7zpp-TestApp\\TestFiles\\Readme.txt"
#define TESTEXTRACTTESTFILE4 SOLUTIONDIR L"7zpp-TestApp\\TestFiles\\7z.zip"

//
// Test loading DLL
//
TEST(Init, LoadDLL)
{
	SevenZip::SevenZipLibrary lib;
	bool result = lib.Load(SevenZip::TString(DLL_PATH));

	ASSERT_EQ(true, result);
}

//
// Test extraction - Test 1
//
TEST(Extract, ExtractFiles_Test1)
{
	SevenZip::SevenZipLibrary lib;
	bool result = lib.Load(SevenZip::TString(DLL_PATH));

	// Make sure DLL loads
	ASSERT_EQ(true, result);

	SevenZip::TString myArchive(std::wstring(TESTEXTRACTTESTFILE1));
	SevenZip::TString myDest(TEMPDIR);

	boost::filesystem::create_directory(TEMPDIR);

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

	// Should have detected a Zip file
	EXPECT_EQ(SevenZip::CompressionFormat::Zip, myCompressionFormat);

	size_t numberofitems = extractor.GetNumberOfItems();

	// Should have found 11 files
	EXPECT_EQ(11, numberofitems);

	std::vector<std::wstring> itemnames = extractor.GetItemsNames();
	std::vector<size_t> origsizes = extractor.GetOrigSizes();

	// Set up expected names and sizes
	std::vector<std::wstring> expecteditemnames;
	std::vector<size_t> expectedorigsizes;

	expecteditemnames.push_back(std::wstring(L"DLL.cpp"));
	expectedorigsizes.push_back(1840);

	expecteditemnames.push_back(std::wstring(L"DLL.h"));
	expectedorigsizes.push_back(1310);

	expecteditemnames.push_back(std::wstring(L"ErrorMsg.cpp"));
	expectedorigsizes.push_back(1463);

	expecteditemnames.push_back(std::wstring(L"ErrorMsg.h"));
	expectedorigsizes.push_back(227);

	expecteditemnames.push_back(std::wstring(L"FileDir.cpp"));
	expectedorigsizes.push_back(15605);

	expecteditemnames.push_back(std::wstring(L"FileDir.h"));
	expectedorigsizes.push_back(2572);

	expecteditemnames.push_back(std::wstring(L"FileFind.cpp"));
	expectedorigsizes.push_back(18771);

	expecteditemnames.push_back(std::wstring(L"FileFind.h"));
	expectedorigsizes.push_back(4561);

	expecteditemnames.push_back(std::wstring(L"FileIO.cpp"));
	expectedorigsizes.push_back(12076);

	expecteditemnames.push_back(std::wstring(L"FileIO.h"));
	expectedorigsizes.push_back(6367);

	expecteditemnames.push_back(std::wstring(L"FileLink.cpp"));
	expectedorigsizes.push_back(10029);

	// Check expected sizes
	for (int i = 0; i < itemnames.size(); i++)
	{
		EXPECT_EQ(expecteditemnames[i], itemnames[i]);
		EXPECT_EQ(expectedorigsizes[i], origsizes[i]);
	}

	//
	//  Using callbacks
	//
	extractor.SetCompressionFormat(SevenZip::CompressionFormat::Zip);
	result = extractor.ExtractArchive(myDest, nullptr);

	EXPECT_EQ(true, result);

	//
	// Look for the actual files
	//
	int i = 0;
	boost::filesystem::recursive_directory_iterator itr(TEMPDIR);
	while (itr != boost::filesystem::recursive_directory_iterator())
	{
		boost::filesystem::path myPath = itr->path().leaf();
		EXPECT_EQ(expecteditemnames[i], myPath.wstring());
		i++;
		++itr;
	}

	// Get rid of our temp directory
	boost::filesystem::remove_all(TEMPDIR);
}

//
// Test extraction - Test 2
//
TEST(Extract, ExtractFiles_Test2)
{
	SevenZip::SevenZipLibrary lib;
	bool result = lib.Load(SevenZip::TString(DLL_PATH));

	// Make sure DLL loads
	ASSERT_EQ(true, result);

	SevenZip::TString myArchive(TESTEXTRACTTESTFILE2);
	SevenZip::TString myDest(TEMPDIR);

	boost::filesystem::create_directory(TEMPDIR);

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

	// Should have detected a GZip file
	EXPECT_EQ(SevenZip::CompressionFormat::GZip, myCompressionFormat);

	size_t numberofitems = extractor.GetNumberOfItems();

	// Should have found 1 file
	EXPECT_EQ(1, numberofitems);

	std::vector<std::wstring> itemnames = extractor.GetItemsNames();
	std::vector<size_t> origsizes = extractor.GetOrigSizes();

	// Set up expected names and sizes
	std::vector<std::wstring> expecteditemnames;
	std::vector<size_t> expectedorigsizes;

	expecteditemnames.push_back(std::wstring(L"ReadMe.txt"));
	expectedorigsizes.push_back(119);

	// Check expected sizes
	for (int i = 0; i < itemnames.size(); i++)
	{
		EXPECT_EQ(expecteditemnames[i], itemnames[i]);
		EXPECT_EQ(expectedorigsizes[i], origsizes[i]);
	}

	//
	//  Using callbacks
	//
	extractor.SetCompressionFormat(SevenZip::CompressionFormat::GZip);
	result = extractor.ExtractArchive(myDest, nullptr);

	EXPECT_EQ(true, result);

	//
	// Look for the actual files
	//
	int i = 0;
	boost::filesystem::recursive_directory_iterator itr(TEMPDIR);
	while (itr != boost::filesystem::recursive_directory_iterator())
	{
		boost::filesystem::path myPath = itr->path().leaf();
		EXPECT_EQ(expecteditemnames[i], myPath.wstring());
		i++;
		++itr;
	}

	// Get rid of our temp directory
	boost::filesystem::remove_all(TEMPDIR);
}

//
// Test extraction - Test 3
//
TEST(Extract, ExtractFiles_Test3)
{
	SevenZip::SevenZipLibrary lib;
	bool result = lib.Load(SevenZip::TString(DLL_PATH));

	// Make sure DLL loads
	ASSERT_EQ(true, result);

	SevenZip::TString myArchive(TESTEXTRACTTESTFILE3);

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

	// Should have detected an Unknown file
	EXPECT_EQ(SevenZip::CompressionFormat::Unknown, myCompressionFormat);
}

//
// Test extraction - Test 4
//
TEST(Extract, ExtractFiles_Test4)
{
	SevenZip::SevenZipLibrary lib;
	bool result = lib.Load(SevenZip::TString(DLL_PATH));

	// Make sure DLL loads
	ASSERT_EQ(true, result);

	SevenZip::TString myArchive(std::wstring(TESTEXTRACTTESTFILE4));
	SevenZip::TString myDest(TEMPDIR);

	boost::filesystem::create_directory(TEMPDIR);

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

	// Should have detected a Zip file
	EXPECT_EQ(SevenZip::CompressionFormat::Zip, myCompressionFormat);

	size_t numberofitems = extractor.GetNumberOfItems();

	// Should have found 1,215 files
	EXPECT_EQ(1215, numberofitems);

	std::vector<std::wstring> itemnames = extractor.GetItemsNames();
	std::vector<size_t> origsizes = extractor.GetOrigSizes();

	// Set up first few expected names and sizes
	std::vector<std::wstring> expecteditemnames;
	std::vector<size_t> expectedorigsizes;

	expecteditemnames.push_back(std::wstring(L"7z\\.git"));
	expectedorigsizes.push_back(27);

	expecteditemnames.push_back(std::wstring(L"7z\\.gitignore"));
	expectedorigsizes.push_back(145);

	expecteditemnames.push_back(std::wstring(L"7z\\Asm"));
	expectedorigsizes.push_back(0);

	expecteditemnames.push_back(std::wstring(L"7z\\Asm\\arm"));
	expectedorigsizes.push_back(0);

	expecteditemnames.push_back(std::wstring(L"7z\\Asm\\arm\\7zCrcOpt.asm"));
	expectedorigsizes.push_back(1466);

	// Check expected sizes of the first few files
	for (int i = 0; i < expecteditemnames.size(); i++)
	{
		EXPECT_EQ(expecteditemnames[i], itemnames[i]);
		EXPECT_EQ(expectedorigsizes[i], origsizes[i]);
	}

	//
	//  Using callbacks
	//
	extractor.SetCompressionFormat(SevenZip::CompressionFormat::Zip);
	result = extractor.ExtractArchive(myDest, nullptr);

	EXPECT_EQ(true, result);

	//
	// Look for the first few actual files
	//
	int i = 0;
	boost::filesystem::recursive_directory_iterator itr(TEMPDIR);
	itr++; // Get rid of the parent directory
	while (itr != boost::filesystem::recursive_directory_iterator() && i < expecteditemnames.size())
	{
		boost::filesystem::path myPath = itr->path();
		std::wstring myActualPath = std::wstring(TEMPDIR) + std::wstring(L"\\") + expecteditemnames[i];
		EXPECT_EQ(myActualPath, myPath.wstring());
		i++;
		++itr;
	}

	//
	//  The line below causes an error with Boost 1.60.  
	//  The directories seem to be read only and all the files are deleted,
	//  but the directory is not deleted for some reason.
	//
	//  It works fine in Test1 and Test2 for some reason...
	//
	//  The temporary workaround is to delete the directory by hand if needed.
	//
	//  I noticed that del /S /Q tmp does not work in Windows 10 either, and that may be the problem.
	//

	// Get rid of our temp directory
	boost::filesystem::remove_all(TEMPDIR);
}

// 
// Test compression
//
TEST(Compress, CompressFiles)
{
	// Place tests here as needed
}

//
// Test listing - Test 1
//
//
// Lister callback
//
class ListCallBackOutput : SevenZip::ListCallback
{
	virtual void OnFileFound(WCHAR* path, int size)
	{
		//std::wcout
		//	<< path
		//	<< L" "
		//	<< size
		//	<< std::endl;
	}
};

TEST(List, ListFiles_Test1)
{
	SevenZip::SevenZipLibrary lib;
	bool result = lib.Load(SevenZip::TString(DLL_PATH));

	// Make sure DLL loads
	ASSERT_EQ(true, result);

	SevenZip::TString myArchive(TESTEXTRACTTESTFILE1);

	//
	// Lister
	//
	ListCallBackOutput myListCallBack;

	SevenZip::SevenZipLister lister(lib, myArchive);
	lister.SetCompressionFormat(SevenZip::CompressionFormat::Zip);
	result = lister.ListArchive((SevenZip::ListCallback *)&myListCallBack);

	EXPECT_EQ(true, result);
}

//
// Main
//
int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

