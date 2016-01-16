#pragma once


#include "SevenZipCompressor.h"
#include "SevenZipExtractor.h"
#include "SevenZipLister.h"

// Version of this library
#define SEVENZIP_VERSION L"201601016.1"
#define SEVENZIP_BRANCH L"master"

#ifdef _DEBUG
	#ifdef _UNICODE
		#pragma comment ( lib, "7zpp_ud.lib" )
	#else
		#pragma comment ( lib, "7zpp_ad.lib" )
	#endif
#else
	#ifdef _UNICODE
		#pragma comment ( lib, "7zpp_u.lib" )
	#else
		#pragma comment ( lib, "7zpp_a.lib" )
	#endif
#endif
