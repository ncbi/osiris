noinst_LIBRARIES = libwxplotctrl.a
libwxplotctrl_a_SOURCES = \
src/fourier.c \
src/fpoptimizer.cc \
src/fparser.cpp \
src/lm_lsqr.cpp \
src/plotctrl.cpp \
src/plotcurv.cpp \
src/plotdata.cpp \
src/plotdraw.cpp \
src/plotfunc.cpp \
src/plotmark.cpp \
src/plotprnt.cpp
AM_CPPFLAGS = -I../wxthings/include -I./include `${HOME}/local/bin/wx-config --cxxflags`
AM_CFLAGS = -I./include
AM_LDFLAGS = -L../wxthings -lwxthings `${HOME}/local/bin/wx-config --libs`
