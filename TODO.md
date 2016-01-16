Things that could be improved: 
- Pushing 0->100% progress callback form the ue wrapper into 7zpp lib. Adjusting code so that it will work with SetCompleted (this function works a bit inconsistently, so I'm doing progress per file for now which is acceptable).
- Support for password protected files

Wishlist:
- Is it possible to do threading in the library, or do we need a .dll?
- Move away form .dll into cross platform support
- Create ability to uncompress parts of items into memory
- Create ability to compress memory into archive files
- Cmake support?

Some links that could help:
- http://sourceforge.net/p/sevenzip/discussion/45797/thread/5e5fc681/?limit=25
- http://sourceforge.net/p/sevenzipjbind/discussion/757964/thread/b64a36fb/
- https://www.autoitscript.com/forum/topic/163421-7zdll-extract-file-to-memory/
