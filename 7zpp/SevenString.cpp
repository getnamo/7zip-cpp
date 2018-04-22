#pragma once

#include "stdafx.h"

#include "SevenString.h"

namespace SevenZip
{

	BSTR TStringAllocSysString(const TString& string)
	{
#ifdef _UNICODE
		return ::SysAllocString(string.c_str());
#else
		int nLen = MultiByteToWideChar(_AtlGetConversionACP(), 0, string.c_str(), (int)string.length(), nullptr, 0);
		BSTR bstr = ::SysAllocStringLen(nullptr, nLen);
		if (bstr != nullptr)
		{
			MultiByteToWideChar(_AtlGetConversionACP(), 0, string.c_str(), (int)string.length(), bstr, nLen);
		}

		return bstr;
#endif
	}

	SevenZip::TString BstrToTString(BSTR bstr)
	{
#ifdef _UNICODE
		return bstr;
#else
		std::vector<char> buffer;
		UINT bstrLen = SysStringLen(bstr);
		int nLen = WideCharToMultiByte(_AtlGetConversionACP(), 0, bstr, bstrLen, nullptr, 0, nullptr, nullptr);
		if (nLen != 0)
		{
			buffer.resize(nLen + 1);
			WideCharToMultiByte(_AtlGetConversionACP(), 0, bstr, bstrLen, buffer.data(), nLen, nullptr, nullptr);
		}

		return buffer.data();
#endif
	}
}