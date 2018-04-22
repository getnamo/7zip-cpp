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
		int nLen = MultiByteToWideChar(_AtlGetConversionACP(), 0, string.c_str(), string.length(), nullptr, 0);
		BSTR bstr = ::SysAllocStringLen(nullptr, nLen);
		if (bstr != nullptr)
		{
			MultiByteToWideChar(_AtlGetConversionACP(), 0, string.c_str(), string.length(), bstr, nLen);
		}

		return bstr;
#endif
	}
}