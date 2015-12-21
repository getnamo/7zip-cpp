// This file is based on the following file from the LZMA SDK (http://www.7-zip.org/sdk.html):
//   ./CPP/7zip/UI/Client7z/Client7z.cpp
#pragma once


#include <7zip/Archive/IArchive.h>
#include <7zip/IPassword.h>

#include "ProgressCallback.h"

//#include "7zip/Archive/IArchive.h"


namespace SevenZip
{
namespace intl
{
	class ArchiveExtractCallback : public IArchiveExtractCallback, public ICryptoGetTextPassword
	{
	private:

		long m_refCount;
		CComPtr< IInArchive > m_archiveHandler;
		TString m_directory;

		TString m_relPath;
		TString m_absPath;
		bool m_isDir;

		bool m_hasAttrib;
		UInt32 m_attrib;

		bool m_hasModifiedTime;
		FILETIME m_modifiedTime;

		bool m_hasNewFileSize;
		UInt64 m_newFileSize;

		ProgressCallback* m_callback;

	public:

		ArchiveExtractCallback( const CComPtr< IInArchive >& archiveHandler, const TString& directory, ProgressCallback* callback );
		virtual ~ArchiveExtractCallback();

		STDMETHOD(QueryInterface)( REFIID iid, void** ppvObject );
		STDMETHOD_(ULONG, AddRef)();
		STDMETHOD_(ULONG, Release)();

		// IProgress
		STDMETHOD(SetTotal)( UInt64 size );
		STDMETHOD(SetCompleted)( const UInt64* completeValue );

		// IArchiveExtractCallback
		STDMETHOD(GetStream)( UInt32 index, ISequentialOutStream** outStream, Int32 askExtractMode );
		STDMETHOD(PrepareOperation)( Int32 askExtractMode );
		STDMETHOD(SetOperationResult)( Int32 resultEOperationResult );

		// ICryptoGetTextPassword
		STDMETHOD(CryptoGetTextPassword)( BSTR* password );

	private:

		void GetPropertyFilePath( UInt32 index );
		void GetPropertyAttributes( UInt32 index );
		void GetPropertyIsDir( UInt32 index );
		void GetPropertyModifiedTime( UInt32 index );
		void GetPropertySize( UInt32 index );

		void EmitDoneCallback();
		void EmitFileDoneCallback(const TString& path);
	};
}
}
