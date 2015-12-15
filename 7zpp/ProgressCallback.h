#pragma once


#include "SevenZipLibrary.h"
#include "CompressionFormat.h"


namespace SevenZip
{
	class ProgressCallback
	{
	public:
		/*
		Called Whenever progress has updated with a float 0-100.0
		*/
		virtual void OnProgress(float progress) {}

		/*
		Called When progress has reached 100%
		*/
		virtual void OnDone() {}
	};
}
