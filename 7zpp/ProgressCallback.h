#pragma once


#include "SevenZipLibrary.h"
#include "CompressionFormat.h"


namespace SevenZip
{
	class ProgressCallback
	{
	public:

		/*
		Called at beginning
		*/
		virtual void OnStartWithTotal(unsigned __int64 totalBytes) {}

		/*
		Called Whenever progress has updated with a bytes complete
		*/
		virtual void OnProgress(unsigned __int64 bytesCompleted) {}

		/*
		Called When progress has reached 100%
		*/
		virtual void OnDone() {}

		/*
		Called When single file progress has reached 100%, returns the filepath that completed
		*/
		virtual void OnFileDone(TString filePath, unsigned __int64 bytesCompleted) {}
	};
}
