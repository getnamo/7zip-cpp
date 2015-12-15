#pragma once


#include "SevenZipLibrary.h"
#include "CompressionFormat.h"


namespace SevenZip
{
	class ProgressCallback
	{
	public:
		virtual void Progress(float progress);
		virtual void Done();
	};
}
