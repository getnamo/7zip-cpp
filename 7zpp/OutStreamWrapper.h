#pragma once


#include <7zip/IStream.h>


namespace SevenZip
{
namespace intl
{
	class OutStreamWrapper : public IOutStream
	{
	private:

		long				m_refCount;
		CComPtr< IStream >	m_baseStream;

	public:

		OutStreamWrapper( const CComPtr< IStream >& baseStream );
		virtual ~OutStreamWrapper();

		STDMETHOD(QueryInterface)( REFIID iid, void** ppvObject );
		STDMETHOD_(ULONG, AddRef)();
		STDMETHOD_(ULONG, Release)();

		// ISequentialOutStream
		STDMETHOD(Write)( const void* data, UInt32 size, UInt32* processedSize );

		// IOutStream
		STDMETHOD(Seek)( Int64 offset, UInt32 seekOrigin, UInt64* newPosition );
		STDMETHOD(SetSize)( UInt64 newSize );
	};
}
}
