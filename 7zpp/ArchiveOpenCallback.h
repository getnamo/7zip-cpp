// This file is based on the following file from the LZMA SDK (http://www.7-zip.org/sdk.html):
//   ./CPP/7zip/UI/Client7z/Client7z.cpp
#pragma once


#include <7zip/Archive/IArchive.h>
#include <7zip/IPassword.h>


namespace SevenZip
{
namespace intl
{
	class ArchiveOpenCallback : public IArchiveOpenCallback, public ICryptoGetTextPassword
	{
	private:

		long m_refCount;

	public:

		ArchiveOpenCallback();
		virtual ~ArchiveOpenCallback();

		STDMETHOD(QueryInterface)( REFIID iid, void** ppvObject );
		STDMETHOD_(ULONG, AddRef)();
		STDMETHOD_(ULONG, Release)();

		// IArchiveOpenCallback
		STDMETHOD(SetTotal)( const UInt64* files, const UInt64* bytes );
		STDMETHOD(SetCompleted)( const UInt64* files, const UInt64* bytes );

		// ICryptoGetTextPassword
		STDMETHOD(CryptoGetTextPassword)( BSTR* password );
	};
}
}
