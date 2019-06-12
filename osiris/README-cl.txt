Set up debugging

Edit osriscl.m4
The following should be present

# production OPT
#OPT="-O3"
# debug/test OPT
OPT="-O0 -g"

The # comments out a line and ONE of the lines beginning with OPT=
should be commented out.  For debugging use OPT="-O0 -g"

----

Building Osiris without the GUI

1. after a git checkout or after adding or removing source files.  Build make files:

./build-osiris-mac-cl.sh

2. if the previous did now show errors and after every code change, compile:

./tmake-cl.sh

----

Run the debugger

lldb ./TestAnalysisDirectoryLCv2.11/bin/TestAnalysisDirectoryLC

at the prompt type 'r' to run (without 'quotes')

Paste or type the parameters 

lldb tutorial
https://lldb.llvm.org/use/tutorial.html

a couple lldb commands

r - run
bt - call stack
h - help


