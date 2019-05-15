// 7zpp-TestApp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <gtest/gtest.h>
#include <boost\filesystem.hpp>
#include <atltime.h>
#include <locale> 
#include <codecvt>

//  Wrapper
#include "../7zpp/7zpp.h"


#define DLL_PATH  "C:\\Program Files\\7-Zip\\7z.dll"
std::string TEMPDIR;
#define ARCHIVE_NAME1  "MyArchive"
#define ARCHIVE_NAME2  "MemArchive"
#define TESTEXTRACTTESTFILE1  "TestFiles\\files.zip"
#define TESTEXTRACTTESTFILE2  "TestFiles\\Readme.txt.gz"
#define TESTEXTRACTTESTFILE3  "TestFiles\\Readme.txt"
#define TESTEXTRACTTESTFILE4  "TestFiles\\7z.zip"
#define TESTCOMPRESSTESTFILE1  "TestFiles\\Readme.md"
#define TESTCOMPRESSTESTFILE2  "TestFiles\\dir"
#define TESTCOMPRESSTESTFILE3  "MemoryFile"
#define TESTCOMPRESSTESTFILE4  "Dir\\MemoryFile"

//found this code here https://stackoverflow.com/questions/4804298/how-to-convert-wstring-into-string
//setup converter
using convert_type = std::codecvt_utf8<wchar_t>;
std::wstring_convert<convert_type, wchar_t> converter;
//
// Test loading DLL
//
TEST(Init, LoadDLL)
{
	SevenZip::SevenZipLibrary lib;
	bool result = lib.Load(SevenZip::TString(DLL_PATH));

	ASSERT_EQ(true, result);

	boost::filesystem::remove_all(TEMPDIR);
}

//
// Test listing - Test 1
//
//
// Lister callback
//
class ListCallBackOutput : SevenZip::ListCallback
{
public:
	~ListCallBackOutput() = default;

	virtual void OnFileFound(const SevenZip::intl::FileInfo& info)
	{
		m_files.push_back(info);
	}

	virtual void OnListingDone(const SevenZip::TString& path)
	{
	}

	const std::vector<SevenZip::FileInfo>& GetList() const { return m_files; }

protected:
	std::vector<SevenZip::FileInfo> m_files;
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
	result = lister.ListArchive(_T(""), (SevenZip::ListCallback *)&myListCallBack);

	EXPECT_EQ(true, result);

	std::wcout << L"Archive contents:\n" << std::endl;

	for (const auto& info : myListCallBack.GetList())
	{
		
            std::cout << info.FileName
                      << " Size: ";
            std::wcout
                << info.Size
                << L" Packed Size: "
                << info.PackedSize
                << L" Attributes: "
                << info.Attributes
                << L"\n Create Time: "
                << CTime(info.CreationTime).Format(_T("%F %T")).GetString()
                << L" Access Time: "
                << CTime(info.LastAccessTime).Format(_T("%F %T")).GetString()
                << L" Modify Time: "
                << CTime(info.LastWriteTime).Format(_T("%F %T")).GetString()
                << std::endl;
	}
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

	SevenZip::TString myArchive(TESTEXTRACTTESTFILE1);
	SevenZip::TString myDest(TEMPDIR);

	// Get rid of our temp directory
	boost::filesystem::remove_all(TEMPDIR);
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
	std::vector<std::string> expecteditemnames;
	std::vector<size_t> expectedorigsizes;

	expecteditemnames.push_back(std::string("DLL.cpp"));
	expectedorigsizes.push_back(1840);

	expecteditemnames.push_back(std::string("DLL.h"));
	expectedorigsizes.push_back(1310);

	expecteditemnames.push_back(std::string("ErrorMsg.cpp"));
	expectedorigsizes.push_back(1463);

	expecteditemnames.push_back(std::string("ErrorMsg.h"));
	expectedorigsizes.push_back(227);

	expecteditemnames.push_back(std::string("FileDir.cpp"));
	expectedorigsizes.push_back(15605);

	expecteditemnames.push_back(std::string("FileDir.h"));
	expectedorigsizes.push_back(2572);

	expecteditemnames.push_back(std::string("FileFind.cpp"));
	expectedorigsizes.push_back(18771);

	expecteditemnames.push_back(std::string("FileFind.h"));
	expectedorigsizes.push_back(4561);

	expecteditemnames.push_back(std::string("FileIO.cpp"));
	expectedorigsizes.push_back(12076);

	expecteditemnames.push_back(std::string("FileIO.h"));
	expectedorigsizes.push_back(6367);

	expecteditemnames.push_back(std::string("FileLink.cpp"));
	expectedorigsizes.push_back(10029);

	// Check expected sizes
	for (int i = 0; i < itemnames.size(); i++)
	{
		EXPECT_EQ(expecteditemnames[i], converter.to_bytes(itemnames[i]));
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
	boost::filesystem::recursive_directory_iterator itr(TEMPDIR), end;
	while (itr != end && i < expecteditemnames.size())
	{
		boost::filesystem::path myPath = itr->path().leaf();
		EXPECT_EQ(expecteditemnames[i], myPath.string());
		i++;
		++itr;
	}
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

	// Get rid of our temp directory
	boost::filesystem::remove_all(TEMPDIR);
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
	std::vector<std::string> expecteditemnames;
	std::vector<size_t> expectedorigsizes;

	expecteditemnames.push_back(std::string("ReadMe.txt"));
	expectedorigsizes.push_back(119);

	// Check expected sizes
	for (int i = 0; i < itemnames.size(); i++)
	{
		EXPECT_EQ(expecteditemnames[i], converter.to_bytes(itemnames[i]));
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
		EXPECT_EQ(expecteditemnames[i], myPath.string());
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

	SevenZip::TString myArchive(std::string(TESTEXTRACTTESTFILE4));
	SevenZip::TString myDest(TEMPDIR);

	// Get rid of our temp directory
	boost::filesystem::remove_all(TEMPDIR);
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
	std::vector<std::string> expecteditemnames;
	std::vector<size_t> expectedorigsizes;

	expecteditemnames.push_back(std::string("7z\\.git"));
	expectedorigsizes.push_back(27);

	expecteditemnames.push_back(std::string("7z\\.gitignore"));
	expectedorigsizes.push_back(145);

	expecteditemnames.push_back(std::string("7z\\Asm"));
	expectedorigsizes.push_back(0);

	expecteditemnames.push_back(std::string("7z\\Asm\\arm"));
	expectedorigsizes.push_back(0);

	expecteditemnames.push_back(std::string("7z\\Asm\\arm\\7zCrcOpt.asm"));
	expectedorigsizes.push_back(1466);

	// Check expected sizes of the first few files
	for (int i = 0; i < expecteditemnames.size(); i++)
	{
		EXPECT_EQ(expecteditemnames[i],converter.to_bytes(itemnames[i]));
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
	boost::filesystem::recursive_directory_iterator itr(TEMPDIR);
	int i = 0;
	itr++; // Get rid of the parent directory
	for(auto&& it : itr) {
		boost::filesystem::path myPath = it.path();
		std::string myActualPath = std::string(TEMPDIR) + expecteditemnames[i];
		EXPECT_EQ(myActualPath, myPath.string());
	i++;
	if(i+1 > expecteditemnames.size())
	break;
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
	//boost::filesystem::remove_all(TEMPDIR);
}

//
// Test extraction - Test 5
//
TEST(Extract, ExtractFiles_Test5)
{
	SevenZip::SevenZipLibrary lib;
	bool result = lib.Load(SevenZip::TString(DLL_PATH));

	// Make sure DLL loads
	ASSERT_EQ(true, result);

	SevenZip::TString myArchive(std::string(TESTEXTRACTTESTFILE4));
	SevenZip::TString myDest(TEMPDIR);

	// Get rid of our temp directory
	boost::filesystem::remove_all(TEMPDIR);
	boost::filesystem::create_directory(TEMPDIR);

	//
	// Lister
	//
	ListCallBackOutput myListCallBack;

	SevenZip::SevenZipLister lister(lib, myArchive);
	SevenZip::CompressionFormatEnum myCompressionFormat = lister.GetCompressionFormat();
	EXPECT_EQ(SevenZip::CompressionFormat::Zip, myCompressionFormat);

	lister.SetCompressionFormat(myCompressionFormat);
	result = lister.ListArchive(_T(""), (SevenZip::ListCallback *)&myListCallBack);
	EXPECT_EQ(true, result);
	EXPECT_EQ(false, myListCallBack.GetList().empty());

	//
	// Extract
	//
	SevenZip::SevenZipExtractor extractor(lib, myArchive);
	extractor.SetCompressionFormat(myCompressionFormat);
	UINT indces[2] = { 1, 4 };
	result = extractor.ExtractFilesFromArchive(indces, 2, myDest);
	ASSERT_EQ(true, result);

	//
	// Look for the first few actual files
	//
	std::vector<std::string> expecteditemnames;
	std::vector<size_t> expectedorigsizes;

	expecteditemnames.push_back(std::string("7z\\.gitignore"));
	expectedorigsizes.push_back(145);

	expecteditemnames.push_back(std::string("7z\\Asm\\arm\\7zCrcOpt.asm"));
	expectedorigsizes.push_back(1466);

	int i = 0;
	boost::filesystem::recursive_directory_iterator itr(TEMPDIR);
	itr++; // Get rid of the parent directory
	while (itr != boost::filesystem::recursive_directory_iterator())
	{
		if(itr->status().type() != boost::filesystem::directory_file)
		{
			boost::filesystem::path myPath = itr->path();
			std::string myActualPath = std::string(TEMPDIR) +  expecteditemnames[i];
			EXPECT_EQ(myActualPath, myPath.string());
			i++;
		}
		++itr;
	}

	std::vector<BYTE> memBuffer;
	result = extractor.ExtractFileToMemory(4, memBuffer);
	ASSERT_EQ(true, result);

	EXPECT_EQ(1466, memBuffer.size());
}

//
// Test compression
//
TEST(Compress, CompressFiles_Test1)
{
	//
	// Add single file, files by mask and files by mask recursive to ZIP
	//
	SevenZip::SevenZipLibrary lib;
	bool result = lib.Load(SevenZip::TString(DLL_PATH));

	// Make sure DLL loads
	ASSERT_EQ(true, result);

	SevenZip::TString myArchive(ARCHIVE_NAME1);
	SevenZip::TString myDest(TEMPDIR);

	boost::filesystem::remove_all(TEMPDIR);
	boost::filesystem::create_directory(TEMPDIR);

	SevenZip::SevenZipCompressor compressor(lib, myArchive);
	compressor.SetCompressionFormat(SevenZip::CompressionFormat::Zip);
	compressor.SetPassword(_T("test"));
	bool addResult = compressor.AddFile(TESTCOMPRESSTESTFILE1);
	EXPECT_EQ(addResult, true);

	// add files by mask non recursive in folder where no files with this mask
	addResult = compressor.AddFiles(TESTCOMPRESSTESTFILE2, _T("*.cpp"), false);
	EXPECT_EQ(addResult, false);

	addResult = compressor.AddFiles(TESTCOMPRESSTESTFILE2, _T("*.cpp"), true);
	EXPECT_EQ(addResult, true);

	bool compressResult = compressor.DoCompress();
	EXPECT_EQ(compressResult, true);
}

TEST(Compress, CompressFiles_Test2)
{
	//
	// Add subdir, recursive, to 7z
	//
	SevenZip::SevenZipLibrary lib;
	bool result = lib.Load(SevenZip::TString(DLL_PATH));

	// Make sure DLL loads
	ASSERT_EQ(true, result);

	boost::filesystem::remove_all(TEMPDIR);
	boost::filesystem::create_directory(TEMPDIR);

	SevenZip::TString myArchive(ARCHIVE_NAME1);
	SevenZip::TString myDest(TEMPDIR);

	SevenZip::SevenZipCompressor compressor(lib, myArchive);
	compressor.SetCompressionFormat(SevenZip::CompressionFormat::SevenZip);
	bool addResult = compressor.AddFile(TESTCOMPRESSTESTFILE1);
	EXPECT_EQ(addResult, true);
	bool addDirResult = compressor.AddDirectory(TESTCOMPRESSTESTFILE2, true);
	EXPECT_EQ(addDirResult, true);

	bool compressResult = compressor.DoCompress();
	EXPECT_EQ(compressResult, true);
}


TEST(Compress, CompressFiles_Test3)
{
	//
	// Add memory to 7z
	//
	SevenZip::SevenZipLibrary lib;
	bool result = lib.Load(SevenZip::TString(DLL_PATH));

	// Make sure DLL loads
	ASSERT_EQ(true, result);

	SevenZip::TString myArchive(ARCHIVE_NAME2);
	SevenZip::TString myDest(TEMPDIR);

	boost::filesystem::remove_all(TEMPDIR);
	boost::filesystem::create_directory(TEMPDIR);

	SevenZip::SevenZipCompressor compressor(lib, myArchive);
	compressor.SetCompressionFormat(SevenZip::CompressionFormat::SevenZip);

	std::string str = "Just a string in a memory";
	bool addResult = compressor.AddMemory(TESTCOMPRESSTESTFILE3, (void*)str.c_str(), str.size());
	EXPECT_EQ(addResult, true);

	bool compressResult = compressor.DoCompress();
	EXPECT_EQ(compressResult, true);
}
//
// Main
//
int main(int argc, char **argv)
{
	boost::filesystem::path current_path(boost::filesystem::current_path());
  TEMPDIR =current_path.string()+"\\temp\\";
  boost::filesystem::remove_all(TEMPDIR);

  ::testing::InitGoogleTest(&argc, argv);
  int result = RUN_ALL_TESTS();

  boost::filesystem::remove_all(TEMPDIR);

  return result;
}

