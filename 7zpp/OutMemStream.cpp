#include "stdafx.h"

#include "OutMemStream.h"

namespace SevenZip
{
	namespace intl
	{
		COutMemStream::COutMemStream(std::vector< BYTE >& out_buffer)
			: m_buffer(out_buffer)
		{
		}

		COutMemStream::~COutMemStream()
		{
		}

		STDMETHODIMP COutMemStream::QueryInterface(REFIID iid, void** ppvObject)
		{
			if (iid == __uuidof(IUnknown))
			{
				*ppvObject = static_cast<IUnknown*>(this);
				AddRef();
				return S_OK;
			}

			if (iid == IID_ISequentialOutStream)
			{
				*ppvObject = static_cast<ISequentialOutStream*>(this);
				AddRef();
				return S_OK;
			}

			return E_NOINTERFACE;
		}

		STDMETHODIMP_(ULONG) COutMemStream::AddRef()
		{
			return static_cast<ULONG>(InterlockedIncrement(&m_refCount));
		}

		STDMETHODIMP_(ULONG) COutMemStream::Release()
		{
			ULONG res = static_cast<ULONG>(InterlockedDecrement(&m_refCount));
			if (res == 0)
			{
				delete this;
			}
			return res;
		}

		STDMETHODIMP COutMemStream::Write(const void* data, UInt32 size, UInt32* processedSize)
		{
			if (processedSize != nullptr)
			{
				*processedSize = 0;
			}
			if (data == NULL || size == 0)
			{
				return E_FAIL;
			}
			const BYTE* byte_data = static_cast<const BYTE*>(data);
			m_buffer.insert(m_buffer.end(), byte_data, byte_data + size);
			*processedSize = size;

			return S_OK;
		}
	}
}