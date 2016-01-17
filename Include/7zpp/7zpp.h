#pragma once

#include "ListCallback.h"
#include "ProgressCallback.h"

#include "AllowWindowsPlatformTypes.h"

#pragma warning(push)
#pragma warning(disable: 4191) // warning C4191: 'type cast' : unsafe conversion
#pragma warning(disable: 4996) // error C4996: 'GetVersionEx': was declared deprecated

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

// atltransactionmanager.h doesn't use the W equivalent functions, use this workaround
#ifndef DeleteFile
#define DeleteFile DeleteFileW
#endif
#ifndef MoveFile
#define MoveFile MoveFileW
#endif

#include <atlbase.h>

#undef DeleteFile
#undef MoveFile

#include <sphelper.h>

#pragma warning(pop)

#include "SevenZipCompressor.h"
#include "SevenZipExtractor.h"
#include "SevenZipLister.h"

// Version of this library
#define SEVENZIP_VERSION L"0.2.0-20160117.1"
#define SEVENZIP_BRANCH L"master"

#include "HideWindowsPlatformTypes.h"

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

