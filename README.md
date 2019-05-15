[![Build status](https://ci.appveyor.com/api/projects/status/11i6cncafgey8lt3?svg=true)](https://ci.appveyor.com/project/getnamo/7zip-cpp)

# 7zip-cpp
A fork of [SevenZip++](http://bitbucket.org/cmcnab/sevenzip/wiki/Home) for modern builds. Uses cmake to generate build files for desired Visual Studio version, see [setup section for instructions](https://github.com/getnamo/7zip-cpp#using-git-and-cmake).

Uses latest lzma1801 SDK

#### Notes
Originally from:
[http://bitbucket.org/cmcnab/sevenzip/wiki/Home](http://bitbucket.org/cmcnab/sevenzip/wiki/Home)


This is a C++ wrapper for accessing the 7-zip COM-like API in 7z.dll and 7za.dll. This code is heavily based off of the Client7z sample found in the [LZMA SDK](http://www.7-zip.org/sdk.html).

The project itself is a static library.

### Basic Usage

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
compressor.UseAbsolutePaths(false);
compressor.AddFile(targetFile);
compressor.AddDirectory(targetDir);
compressor.DoCompress(callbackfunc);
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
	virtual void OnFileFound(WCHAR* path, ULONGLONG size)
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

## Setup and Installation

### Using git and cmake
1. Ensure you have [cmake](https://cmake.org/download/) and [git](https://git-scm.com/downloads) installed. Navigate to folder of choice.
2. Open a powershell window and type ```git clone https://github.com/getnamo/7zip-cpp.git --recursive```
3. Navigate into the newly cloned project ```cd 7zip-cpp```
4. (Optional for Test app only) Download and build [Boost](https://www.boost.org/users/download/)
5. Example build with cmake using powershell
```plain 
cd build
cmake -G "Visual Studio 15 2017 Win64" ../
cmake --build ../build --config Release
```
6. Commands from 5 will build win64 Release target and output will be found in ```build/Release/7zpp.lib```

### How to use this library in my project
Add project into your cmakelists 

```plain
add_subdirectory(${pathto7zip-cpp} ${PROJECT_SOURCE_DIR}/build/build7zpp)
target_include_directories(${my_project} INTERFACE ${pathto7zip-cpp}/Include)
target_link_libraries(${my_project} 7zpp)
add_dependencies(${my_project}  7zpp) #might not be necessary
```

### Test Requirements

In order to compile the tests,you must have boost libraries in your path or specify the location where cmake can find them

1. Assuming you're in the project directory
2. Download and build [Boost](https://www.boost.org/users/download/)
```plain
cd build
cmake ../ -DBOOST_ROOT="My boost location"
```
2. Then finally `cmake --build ../build` to build


## Known Issues

~~Only 1 test unit is failing.~~

## Contributing

Read Contributing.md

### Branches

- Devel branch is the bleeding edge, but it should still work.
- Devel-XXX branches are current topics.
- Master branch is the latest stable version.
- More branch information is in Contributing.md.
