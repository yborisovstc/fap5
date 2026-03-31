AM_CPPFLAGS = -D_PLAT_LINUX_ -Wall -g -O2

bin_PROGRAMS = fap5srv

fap5srv_SOURCES = \
		       server_main.cpp \
		       requests.h \
		       requests.cpp \
		       fapcsbase.h \
		       fapcsbase.cpp \
		       sclient.h \
		       sclient.cpp \
		       sthread.h \
		       sthread.cpp \
		       sserver.h \
		       sserver.cpp

include_HEADERS = \
		  requests.h \
		  sserver.h \
		  fapcsbase.h

#fap5srv_LDFLAGS=  -L../client/.libs/ -lfap2client -L../dmas/.libs -lfap2da -lrt -Wl,--no-as-needed
fap5srv_LDFLAGS=  -L../src/.libs/ -lfap5 -lrt -Wl,--no-as-needed
fap5srv_CXXFLAGS=-I@srcdir@/../src
#fap5srv_LDFLAGS= -lrt -Wl,--no-as-needed
