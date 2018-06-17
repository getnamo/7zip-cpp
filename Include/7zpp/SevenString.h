#pragma once


#include <tchar.h>
#include <string>
#include <OleAuto.h>

namespace SevenZip
{
#ifdef _UNICODE
	typedef std::wstring TString;
#else
	typedef std::string TString;
#endif
}
namespace SevenZip
{
	BSTR TStringAllocSysString(const TString& string);
	TString BstrToTString(BSTR string);
}
