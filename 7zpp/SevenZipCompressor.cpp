#include "stdafx.h"
#include "SevenZipCompressor.h"
#include "GUIDs.h"
#include "FileSys.h"
#include "ArchiveUpdateCallback.h"
#include "OutStreamWrapper.h"
#include "PropVariant.h"
#include "UsefulFunctions.h"


namespace SevenZip
{

using namespace intl;


const TString SearchPatternAllFiles = _T( "*" );

SevenZipCompressor::SevenZipCompressor(const SevenZipLibrary& library, const TString& archivePath)
	: SevenZipArchive(library, archivePath)
{
}

SevenZipCompressor::~SevenZipCompressor()
{
}

bool SevenZipCompressor::CompressDirectory( const TString& directory, bool includeSubdirs /*= true*/)
{
	return AddFilesToList(directory, SearchPatternAllFiles, FileSys::GetPath( directory ), includeSubdirs);
}

bool SevenZipCompressor::CompressFiles( const TString& directory, const TString& searchFilter, bool includeSubdirs /*= true*/)
{
	return AddFilesToList(directory, searchFilter, directory, includeSubdirs);
}

bool SevenZipCompressor::CompressAllFiles( const TString& directory, bool includeSubdirs /*= true*/)
{
	return AddFilesToList(directory, SearchPatternAllFiles, directory, includeSubdirs);
}

bool SevenZipCompressor::CompressFile( const TString& filePath)
{
	std::vector< FilePathInfo > files = FileSys::GetFile( filePath );

	if ( files.empty() )
	{
		return false;
	}

	m_outputPath = filePath;

	m_fileList.insert(m_fileList.end(), files.begin(), files.end());

	return true;
}

bool SevenZipCompressor::DoCompress(ProgressCallback* callback /*= nullptr*/)
{
	if (m_fileList.empty())
	{
		return false;
	}

	return CompressFilesToArchive(TString(), m_fileList, callback);
}

CComPtr< IStream > SevenZipCompressor::OpenArchiveStream()
{
	CComPtr< IStream > fileStream = FileSys::OpenFileToWrite( m_archivePath );
	if ( fileStream == NULL )
	{
		return nullptr;
	}
	return fileStream;
}

bool SevenZipCompressor::AddFilesToList( const TString& directory, const TString& searchPattern, const TString& pathPrefix, bool recursion)
{
	if ( !FileSys::DirectoryExists( directory ) )
	{
		return false;	//Directory does not exist
	}

	if ( FileSys::IsDirectoryEmptyRecursive( directory ) )
	{
		return false;	//Directory \"%s\" is empty" ), directory.c_str()
	}

	m_outputPath = directory;

	std::vector< FilePathInfo > files = FileSys::GetFilesInDirectory( directory, searchPattern, recursion );
	if (files.empty())
	{
		return false;
	}

	m_fileList.insert(m_fileList.end(), files.begin(), files.end());

	return true;
}

bool SevenZipCompressor::CompressFilesToArchive(const TString& pathPrefix, const std::vector< FilePathInfo >& filePaths, ProgressCallback* progressCallback /*= nullptr*/)
{
	CComPtr< IOutArchive > archiver = UsefulFunctions::GetArchiveWriter(m_library, m_compressionFormat);
	if (!archiver)
	{
		// compression not supported
		return false;
	}

	SetCompressionProperties( archiver );

	//Set full outputFilePath including ending
	m_outputPath += UsefulFunctions::EndingFromCompressionFormat(m_compressionFormat);

	CComPtr< OutStreamWrapper > outFile = new OutStreamWrapper( OpenArchiveStream() );
	CComPtr< ArchiveUpdateCallback > updateCallback = new ArchiveUpdateCallback( pathPrefix, filePaths, m_outputPath, progressCallback );

	HRESULT hr = archiver->UpdateItems( outFile, (UInt32) filePaths.size(), updateCallback );

	if (progressCallback)
	{
		progressCallback->OnDone(m_outputPath);	//Todo: give full path support
	}

	// returning S_FALSE also indicates error
	return (hr == S_OK) ? true : false;
}

bool SevenZipCompressor::SetCompressionProperties( IUnknown* outArchive )
{
	if (!outArchive)
	{
		return false;
	}

	const size_t numProps = 1;
	const wchar_t* names[numProps] = { L"x" };
	CPropVariant values[numProps] = { static_cast< UInt32 >( m_compressionLevel.GetValue() ) };

	CComPtr< ISetProperties > setter;
	outArchive->QueryInterface( IID_ISetProperties, reinterpret_cast< void** >( &setter ) );
	if ( setter == nullptr )
	{
		return false;	//Archive does not support setting compression properties
	}

	HRESULT hr = setter->SetProperties( names, values, numProps );

	// returning S_FALSE also indicates error
	return (hr == S_OK) ? true : false;
}

}
