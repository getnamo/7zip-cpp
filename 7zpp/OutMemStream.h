#pragma once

#include <vector>

#include <7zip/IStream.h>

namespace SevenZip
{
	namespace intl
	{
		class COutMemStream : public ISequentialOutStream
		{
		public:

			explicit COutMemStream(std::vector< BYTE >& out_buffer);
			virtual ~COutMemStream();

			STDMETHOD(QueryInterface)(REFIID iid, void** ppvObject);
			STDMETHOD_(ULONG, AddRef)();
			STDMETHOD_(ULONG, Release)();

			// ISequentialOutStream
			STDMETHOD(Write)(const void* data, UInt32 size, UInt32* processedSize);

		private:

			long m_refCount;
			std::vector<BYTE>& m_buffer;
		};
	}
}
