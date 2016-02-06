# OSIRIS

A public domain quality assurance software package that facilitates the assessment of multiplex short tandem repeat (STR) DNA profiles based on laboratory-specific protocols. OSIRIS evaluates the raw electrophoresis data contained in .fsa or .hid files using an independently derived mathematically-based sizing algorithm.  OSIRIS currently supports ABI capillary analytical platforms and numerous commercially available marker kits including all CODIS-compliant kits as well as those favored by biomedical laboratories. 

## MS-Windows build

### Requirements and tools
* Most of the requirements are included with OSIRIS, except for [wxwidgets](http://www.wxwidgets.org/). These were built with multithreading but not DLL, i.e. `/MT` as opposed to `/MD` with a few modifications. 
* VC++ 2010 Version 10 - In visual C++, see `Help` -> `About Microsoft Visual Studio`), it should be 10 or later to build OSIRIS.
* wxWidgets (http://www.wxwidgets.org/)
* [`libxml2` and `libxslt`](http://xmlsoft.org/)
* [`libiconv`](http://www.gnu.org/software/libiconv/)
* [Header files for all three libraries for MC VC++](http://www.zlatkovic.com/libxml.en.html), used to replace the corresponding header files in the distribution.
* Modification (already done) to these libraries
** In `libxslt-1.1.26/libexslt`, modified `libexslt.h` and changed `#include <win32config.h>` to `#include <libxslt/win32config.h>`
** Removed all occurrences of `#include <win32config.h>` from all other `\*.c`, `\*.h` files and made sure that `#include "libexslt/libexslt.h"` is present.
** Created VC++ projects and build the the code generation option of "Multithreaded" (`/MT`).

### Build steps
* Download wxWidgets at http://wxwidgets.org/downloads/, either  the ZIP or 7z download.
* [Download the source code for OSIRIS version 2.5](https://github.com/amanjeev/osiris/archive/Version-2.5.zip) (or 2.4 or later) from [Github](https://github.com/ncbi/osiris) (Version 2.3 requires an older version of `wxWidgets`). Or select the version from the `Branch` pulldown on the left side of the web page and select `Download ZIP` from the right side.  

#### Build wxWidgets
* Create a new folder, (for example `wxWidgets-3.0.2`) and unpack the contents of the zip file there.
* Set the environment variable `WXHOME`, to the folder containing wxWidgets 3.0.2 (This is done in Control Panel, search for "environment").
* There should exist `%WXHOME%\include` and `%WXHOME%\lib`.
* Go to the wxWidgets `build` folder, then `msw`.
* Select the appropriate solution `.sln` file for your version of Visual C++.
* Build in Visual C++ for "Release" and `Win32` and optionally for `Debug` and `Win32`. Both can coexist in the same directory structure.

#### Build OSIRIS
* OSIRIS is built with Microsoft Visual C++ from Microsoft Visual Studio  2010 Professional.  Use of a newer version of Visual C++ may result in compiler errors.
* It is important that the Visual C++ environment is started **after** setting `WXHOME` in the environment.
* Make sure you are building for `Release` and not `Debug`.
* Unpack the zip file and go to the `osiris` folder.
* Open the solution file, `osirisAll.sln` with Visual C++ 2010 (or later).
* Select `Release` and `Win32` in the toolbar or from the menubar: `Build` -> `Configuration Manager`
* Build the solution:  From the menubar: `Build` -> `Rebuild Solution` (Optional - If you built wxWidgets for "Debug" you can also build OSIRIS for "Debug"  however it recommended that you copy the directory structure of an OSIRIS installation to OsirisAnalysis/Debug except for `OsirisAnalysis.exe`).

#### Build OSIRIS - Alternative
* Run the perl script `.\ZipPerl\MakeZip.pl` 
** Requires [cygwin](http://www.cygwin.com/), [Perl](http://www.activestate.com/), and [7zip](http://www.7-zip.org/).
** After the above are installed, the commands, 7z and cp must be available from `PATH` environment variable.
* It will set up the directory structure in `.\ZipPerl\Osiris`
* Then create a zip file.
* If you want to create the directory structure without a zip file, then comment out the line of code in `MakeZip.pl` containing '7z' by preceeding it with a pound sign (#) like the first several lines of this file.
* The directory structure will be in `.\ZipPerl\Osiris` and can be moved.

## Mac OS X build

### Requirements and tools
* Mac OS X 10.9 to 10.10 build.
* Unix command line.
* [Mac OS X Developer tools](https://developer.apple.com/xcode/) - try `gcc --version` to see if you have `gcc` available from the tools.

### Build steps
* [Download the source code for OSIRIS version 2.5](https://github.com/amanjeev/osiris/archive/Version-2.5.zip) and unzip it to `osiris` directory.
* Download wxWidgets at http://wxwidgets.org/downloads/, `Source for Linux, OS X, etc.` ([Current](https://github.com/wxWidgets/wxWidgets/releases/download/v3.0.2/wxWidgets-3.0.2.tar.bz2)).

#### Build and install wxWidgets
* Unpack the download to a new directory: `tar tvf wxWidgets-3.0.2.tar.bz2`. This will create a directory with the wxWidget source code like `wxWidgets-3.0.2`
* Change into the directory: `cd wxWidgets-3.0.2`
* Create `build-cocoa`: `mkdir build-cocoa`
* Change into the directory: `cd build-cocoa`
* In `osiris` directory, copy `osiris/osiris/build-osx/config.sh` to `wxWidgets-3.0.2/build-cocoa`: `cp /path/to/osiris/osiris/build-osx/config.sh .`
* Run the newly copied `config.sh`: `sh config.sh`
** When finished, the output will be in the file `build_log.txt`
** Make sure there are no error messages at the end of the file.
* Install `wxWidgets`: `make install`
**  The default installation location from the build script is  `~/local/wxRelease` (where `~` represents the user's home directory).
** Make sure that `~/local/wxRelease` has four subdirectories: `bin`, `include`, `lib`, `share`
** Set the environment variation for wxWidgets: `export WXHOME=~/local/wxRelease`

#### Build and install OSIRIS
* Run the `osiris/build-osiris-mac-osx.sh`: `sh build-osiris-mac-osx.sh`
* If there are no errors, then run `make`.
** There will be a lot of warnings.
* If there are no errors run `install-mac-osx.sh`: `sh install-mac-osx.sh`.
** this installs Osiris in `~/Applications` and create a distribution file, `Osiris-Mac-2.x.tar.gz` in the this folder.
* Open `Osiris.app` to run OSIRIS.

