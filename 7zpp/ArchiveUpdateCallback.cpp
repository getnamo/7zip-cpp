// This file is based on the following file from the LZMA SDK (http://www.7-zip.org/sdk.html):
//   ./CPP/7zip/UI/Client7z/Client7z.cpp
#include "StdAfx.h"
#include "ArchiveUpdateCallback.h"
#include "PropVariant.h"
#include "FileSys.h"
#include "InStreamWrapper.h"


namespace SevenZip
{
namespace intl
{

ArchiveUpdateCallback::ArchiveUpdateCallback( const TString& dirPrefix, const std::vector< FilePathInfo >& filePaths, const TString& outputFilePath, ProgressCallback* callback)
	: m_refCount( 0 )
	, m_dirPrefix( dirPrefix )
	, m_filePaths( filePaths )
	, m_callback(callback)
	, m_outputPath(outputFilePath)
{
}

ArchiveUpdateCallback::~ArchiveUpdateCallback()
{
}

STDMETHODIMP ArchiveUpdateCallback::QueryInterface( REFIID iid, void** ppvObject )
{
	if ( iid == __uuidof( IUnknown ) )
	{
		*ppvObject = reinterpret_cast< IUnknown* >( this );
		AddRef();
		return S_OK;
	}

	if ( iid == IID_IArchiveUpdateCallback )
	{
		*ppvObject = static_cast< IArchiveUpdateCallback* >( this );
		AddRef();
		return S_OK;
	}

	if ( iid == IID_ICryptoGetTextPassword2 )
	{
		*ppvObject = static_cast< ICryptoGetTextPassword2* >( this );
		AddRef();
		return S_OK;
	}

	if ( iid == IID_ICompressProgressInfo )
	{
		*ppvObject = static_cast< ICompressProgressInfo* >( this );
		AddRef();
		return S_OK;
	}

	return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) ArchiveUpdateCallback::AddRef()
{
	return static_cast< ULONG >( InterlockedIncrement( &m_refCount ) );
}

STDMETHODIMP_(ULONG) ArchiveUpdateCallback::Release()
{
	ULONG res = static_cast< ULONG >( InterlockedDecrement( &m_refCount ) );
	if ( res == 0 )
	{
		delete this;
	}
	return res;
}

STDMETHODIMP ArchiveUpdateCallback::SetTotal( UInt64 size )
{
	if (m_callback != nullptr)
	{
		m_callback->OnStartWithTotal(m_outputPath, size);
	}
	return S_OK;
}

STDMETHODIMP ArchiveUpdateCallback::SetCompleted( const UInt64* completeValue )
{
	if (m_callback!=nullptr)
	{
		m_callback->OnProgress(m_outputPath, *completeValue);
	}
	return S_OK;
}

STDMETHODIMP ArchiveUpdateCallback::GetUpdateItemInfo( UInt32 index, Int32* newData, Int32* newProperties, UInt32* indexInArchive )
{
	// Setting info for Create mode (vs. Append mode).
	// TODO: support append mode
	if ( newData != NULL )
	{
		*newData = 1;
	}

	if ( newProperties != NULL )
	{
		*newProperties = 1;
	}

	if ( indexInArchive != NULL )
	{
		*indexInArchive = static_cast< UInt32 >( -1 ); // TODO: UInt32.Max
	}

	return S_OK;
}

STDMETHODIMP ArchiveUpdateCallback::GetProperty( UInt32 index, PROPID propID, PROPVARIANT* value )
{
	CPropVariant prop;

	if ( propID == kpidIsAnti )
	{
		prop = false;
		prop.Detach( value );
		return S_OK;
	}

	if ( index >= m_filePaths.size() )
	{
		return E_INVALIDARG;
	}

	const FilePathInfo& fileInfo = m_filePaths.at( index );
	switch ( propID )
	{
		case kpidPath:		prop = FileSys::ExtractRelativePath( m_dirPrefix, fileInfo.FilePath ).c_str(); break;
		case kpidIsDir:		prop = fileInfo.IsDirectory; break;
		case kpidSize:		prop = fileInfo.Size; break;
		case kpidAttrib:	prop = fileInfo.Attributes; break;
		case kpidCTime:		prop = fileInfo.CreationTime; break;
		case kpidATime:		prop = fileInfo.LastAccessTime; break;
		case kpidMTime:		prop = fileInfo.LastWriteTime; break;
	}

	prop.Detach( value );
	return S_OK;
}

STDMETHODIMP ArchiveUpdateCallback::GetStream( UInt32 index, ISequentialInStream** inStream )
{
	if ( index >= m_filePaths.size() )
	{
		return E_INVALIDARG;
	}

	const FilePathInfo& fileInfo = m_filePaths.at( index );
	if ( fileInfo.IsDirectory )
	{
		return S_OK;
	}

	CComPtr< IStream > fileStream = FileSys::OpenFileToRead( fileInfo.FilePath );
	if ( fileStream == NULL )
	{
		return HRESULT_FROM_WIN32( GetLastError() );
	}

	CComPtr< InStreamWrapper > wrapperStream = new InStreamWrapper( fileStream );
	*inStream = wrapperStream.Detach();

	return S_OK;
}

STDMETHODIMP ArchiveUpdateCallback::SetOperationResult( Int32 operationResult )
{
	return S_OK;
}

STDMETHODIMP ArchiveUpdateCallback::CryptoGetTextPassword2( Int32* passwordIsDefined, BSTR* password )
{
	// TODO: support passwords
	*passwordIsDefined = 0;
	*password = SysAllocString( L"" );
	return *password != 0 ? S_OK : E_OUTOFMEMORY;
}

STDMETHODIMP ArchiveUpdateCallback::SetRatioInfo( const UInt64* inSize, const UInt64* outSize )
{
	return S_OK;
}

}
}
