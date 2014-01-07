bin_PROGRAMS = wxplotctrl
wxplotctrl_SOURCES = wxplotctrl.cpp
INCLUDES = -I../../../wxthings/include -I../../include 
AM_LDFLAGS = -L../../../wxthings -L../.. -lwxthings -lwxplotctrl `${HOME}/local/bin/wx-config --libs`
AM_CPPFLAGS = -msse2 `${HOME}/local/bin/wx-config --cxxflags`

./PlotCtrlSample.app/Contents/MacOS/wxplotctrl: wxplotctrl
	cp -v -p wxplotctrl ./PlotCtrlSample.app/Contents/MacOS/

app: ./PlotCtrlSample.app/Contents/MacOS/wxplotctrl
	true
