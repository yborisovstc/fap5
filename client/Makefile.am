AM_CPPFLAGS = -D_PLAT_LINUX_ -Wall -g -O2

lib_LTLIBRARIES = libfap5client.la

libfap5client_la_SOURCES = \
                       bclient.h \
                       bclient.cpp \
		       ../server/requests.h \
		       ../server/requests.cpp

include_HEADERS = \
		  bclient.h

libfap5client_la_LDFLAGS = -rdynamic -no-undefined
