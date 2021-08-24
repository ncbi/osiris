bin_PROGRAMS = GenerateLadderFile
AUTOMAKE_OPTIONS = subdir-objects
GenerateLadderFile_SOURCES = ../Bins.cpp \
../GenerateLadderFile.cpp \
../LadderInputFile.cpp \
../LadderStructures.cpp \
../LocusName.cpp \
../NameManager.cpp \
../Panels.cpp
GenerateLadderFile_DEPENDENCIES = ../../BaseClassLib/lib/librgtools.a
AM_CPPFLAGS = -I../../BaseClassLib
LDADD = -L../../BaseClassLib/lib -lrgtools 
