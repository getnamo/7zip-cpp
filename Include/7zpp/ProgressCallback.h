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
		virtual void OnStartWithTotal(const TString& archivePath, unsigned __int64 totalBytes) = 0;

		/*
		Called Whenever progress has updated with a bytes complete
		*/
		virtual void OnProgress(const TString& archivePath, unsigned __int64 bytesCompleted) = 0;


		/*
		Called When progress has reached 100%
		*/
		virtual void OnDone(const TString& archivePath) = 0;

		/*
		Called When single file progress has reached 100%, returns the filepath that completed
		*/
		virtual void OnFileDone(const TString& archivePath, const TString& filePath, unsigned __int64 bytesCompleted) = 0;

		/*
		Called to determine if it's time to abort the zip operation. Return true to abort the current operation.
		*/
		virtual bool OnCheckBreak() = 0;
	};
}
