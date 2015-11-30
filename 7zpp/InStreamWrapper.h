#pragma once


#include <7zip/IStream.h>


namespace SevenZip
{
namespace intl
{
	class InStreamWrapper : public IInStream, public IStreamGetSize
	{
	private:

		long				m_refCount;
		CComPtr< IStream >	m_baseStream;

	public:

		InStreamWrapper( const CComPtr< IStream >& baseStream );
		virtual ~InStreamWrapper();

		STDMETHOD(QueryInterface)( REFIID iid, void** ppvObject );
		STDMETHOD_(ULONG, AddRef)();
		STDMETHOD_(ULONG, Release)();

		// ISequentialInStream
		STDMETHOD(Read)( void* data, UInt32 size, UInt32* processedSize );

		// IInStream
		STDMETHOD(Seek)( Int64 offset, UInt32 seekOrigin, UInt64* newPosition );

		// IStreamGetSize
		STDMETHOD(GetSize)( UInt64* size );
	};
}
}
