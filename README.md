# 7zip-cpp
Fork of SevenZip++ for VS2015

From:
[http://bitbucket.org/cmcnab/sevenzip/wiki/Home](http://bitbucket.org/cmcnab/sevenzip/wiki/Home)


This is a C++ wrapper for accessing the 7-zip COM-like API in 7z.dll and 7za.dll. This code is heavily based off of the Client7z sample found in the [LZMA SDK](http://www.7-zip.org/sdk.html).

The project itself is a static library.

To use, first load the 7z DLL into memory:

```cpp
#include <7zpp/7zpp.h>

SevenZip::SevenZipLibrary lib;
lib.Load();
```

If the appropriate 7z DLL is not in your path you may wish to specify it explicitly in the call to load. Note you may have the 64-bit version installed but are trying to load it from a 32-bit executable; keep that in mind if you encounter errors.

```cpp
lib.Load(_T("path\\to\\7za.dll"));
```

Then create and use a compressor:

```cpp
SevenZip::SevenZipCompressor compressor(lib, archiveName);
compressor.SetCompressionFormat(SevenZip::CompressionFormat::Zip);
compressor.CompressDirectory(targetDir, callbackfunc);
```

Or an extractor:

```cpp
SevenZip::SevenZipExtractor extractor(lib, archiveName);

// Try to detect compression type
if (!extractor.DetectCompressionFormat())
{
	extractor.SetCompressionFormat(SevenZip::CompressionFormat::Zip);
}

...

// Change this function to suit
SevenZip::ProgressCallBack *extractcallbackfunc = nullptr;

...

extractor.ExtractArchive(destination, extractcallbackfunc);
```

Or a lister:

```cpp
class ListCallBackOutput : SevenZip::ListCallback
{
	virtual void OnFileFound(WCHAR* path, int size)
	{
		std::wcout
			<< path
			<< L" "
			<< size
			<< std::endl;
	}
};

...

SevenZip::SevenZipLister lister(lib, archiveName);

// Try to detect compression type
if (!lister.DetectCompressionFormat())
{
	lister.SetCompressionFormat(SevenZip::CompressionFormat::Zip);
}

ListCallBackOutput myListCallBack;


lister.ListArchive((SevenZip::ListCallback *)&myListCallBack);
```

Note:  Most of the functions now return a boolean to indicate if it worked
instead of throwing an exception.

Otherwise, don't forget to wrap the operations in a try/catch block to handle errors:

```cpp
...
catch (SevenZip::SevenZipException& ex)
{
    std::cerr << ex.GetMessage() << std::endl;
}
...
```

## Requirements

In order to compile the tests, the following requirements must be available:

- Install Boost v1.60.0 binaries into $(SolutionDir)\\..\boost_1_60_0\ from http://www.boost.org
- Install GoogleTest into $(SolutionDir)\\..\googletest\ from https://github.com/keithjjones/googletest.git
  - Go into the googletest directory
  - Make directory ```build```
  - Change directory into ```build```
  - Run ```cmake .. -G "Visual Studio 14 2015 Win64"```
  - Open the solution and compile GoogleTest.
- Now you can compile 7zip-cpp

The solution assumes 7zip is installed in ```C:\Program Files\7-Zip\7z.dll```.

## Known Issues

The extractor can have issues with relative paths.  To be sure it works correctly, feed it
a full path.

There is a bug in the testing program.  The issue has been logged on @keithjjones repository.

## Contributing

Read Contributing.md

### Branches

- Devel branch is the bleeding edge, but it should still work.
- Devel-XXX branches are current topics.
- Master branch is the latest stable version.
- More branch information is in Contributing.md.
