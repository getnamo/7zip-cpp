#pragma once


#include "SevenZipLibrary.h"
#include "CompressionFormat.h"


namespace SevenZip
{
	class ListCallback
	{
	public:
		virtual void FileFound(WCHAR* path, int size);
	};
}
