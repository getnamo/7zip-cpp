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
compressor.CompressDirectory(targetDir, callbackfunc);
```

Or an extractor:

```cpp
SevenZip::SevenZipExtractor extractor(lib, archiveName);
extractor.ExtractArchive(destination, callbackfunc);
```

```cpp
SevenZip::SevenZipLister lister(lib, archiveName);
lister.ListArchive(destination, callbackfunc);
```


Don't forget to wrap the operations in a try/catch block to handle errors:

```cpp
...
catch (SevenZip::SevenZipException& ex)
{
    std::cerr << ex.GetMessage() << std::endl;
}

```
Note:  Most of the functions now return a boolean to indicate if it worked
instead of throwing an exception.
