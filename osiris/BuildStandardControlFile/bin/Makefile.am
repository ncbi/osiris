bin_PROGRAMS = BuildStandardControlFile
AUTOMAKE_OPTIONS = subdir-objects
BuildStandardControlFile_SOURCES = \
  ../BuildStdCtlFile.cpp \
  ../LocusData.cpp
BuildStandardControlFile_DEPENDENCIES = ../../BaseClassLib/lib/librgtools.a ../../OsirisLib2.01/lib/libosiris.a
AM_CPPFLAGS = -I../../BaseClassLib -I../../OsirisLib2.01
LDADD = -L../../BaseClassLib/lib ../../OsirisLib2.01/lib -lrgtools -losiris 
