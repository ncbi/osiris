bin_PROGRAMS = TestAnalysisDirectoryLC
AUTOMAKE_OPTIONS = subdir-objects
TestAnalysisDirectoryLC_DEPENDENCIES = \
../../BaseClassLib/lib/librgtools.a \
../../OsirisMath/lib/libOsirisMath.a \
../../OsirisLib2.01/lib/libosiris.a

TestAnalysisDirectoryLC_SOURCES = ../TestAnalysisDirectoryLC.cpp
AM_CPPFLAGS = -I../../BaseClassLib -I../../OsirisLib2.01 -I../../OsirisMath
LDADD = -L../../BaseClassLib/lib -L../../OsirisMath/lib -L../../OsirisLib2.01/lib  -losiris -lOsirisMath -lrgtools 
