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

ArchiveUpdateCallback::ArchiveUpdateCallback(const std::vector< FilePathInfo >& filePaths, const TString& outputFilePath, const TString& password, ProgressCallback* callback)
	: m_refCount(0)
	, m_filePaths(filePaths)
	, m_callback(callback)
	, m_outputPath(outputFilePath)
	, m_password(password)
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
	return CheckBreak();
}

STDMETHODIMP ArchiveUpdateCallback::SetCompleted( const UInt64* completeValue )
{
	if (m_callback!=nullptr)
	{
		m_callback->OnProgress(m_outputPath, *completeValue);
	}
	return CheckBreak();
}

STDMETHODIMP ArchiveUpdateCallback::CheckBreak()
{
	if (m_callback != nullptr)
	{
		// Abort if OnCheckBreak returns true;
		return m_callback->OnCheckBreak() ? E_ABORT : S_OK;
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

	return CheckBreak();
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
		case kpidPath:		prop = FileSys::ExtractRelativePath(fileInfo.rootPath, fileInfo.FilePath).c_str(); break;
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

	HRESULT hr = S_OK;
	CComPtr< IStream > outStream;
	if (fileInfo.memFile)
	{
		HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, (SIZE_T)fileInfo.Size);
		if (!hGlobal)
		{
			return E_OUTOFMEMORY;
		}

		void* lpData = GlobalLock(hGlobal);
		memcpy(lpData, fileInfo.memPointer, (SIZE_T)fileInfo.Size);
		GlobalUnlock(hGlobal);
		hr = CreateStreamOnHGlobal(hGlobal, TRUE, &outStream);
	}
	else
	{
		outStream = FileSys::OpenFileToRead(fileInfo.FilePath);
		hr = HRESULT_FROM_WIN32(GetLastError());
	}

	if (!outStream)
	{
		return hr;
	}

	CComPtr< InStreamWrapper > wrapperStream = new InStreamWrapper(outStream);
	*inStream = wrapperStream.Detach();

	return CheckBreak();
}

STDMETHODIMP ArchiveUpdateCallback::SetOperationResult( Int32 operationResult )
{
	return CheckBreak();
}

STDMETHODIMP ArchiveUpdateCallback::CryptoGetTextPassword2( Int32* passwordIsDefined, BSTR* password )
{
	if (!m_password.empty())
		*password = TStringAllocSysString(m_password);

	*passwordIsDefined = m_password.empty() ? 0 : 1;

	return S_OK;
}

STDMETHODIMP ArchiveUpdateCallback::SetRatioInfo( const UInt64* inSize, const UInt64* outSize )
{
	return CheckBreak();
}

}
}
