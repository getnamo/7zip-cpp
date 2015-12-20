#pragma once

#include "SevenZipLibrary.h"
#include "CompressionFormat.h"
#include <7zip/Archive/IArchive.h>
#include "GUIDs.h"

namespace SevenZip
{
	using namespace intl;

	class UsefulFunctions
	{
	public:
		static const GUID* GetCompressionGUID(const CompressionFormatEnum& format);
		static CComPtr< IInArchive > GetArchiveReader(const SevenZipLibrary& library, const CompressionFormatEnum& format);
	};
}
