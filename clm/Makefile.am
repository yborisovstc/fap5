#
# FAP5 command line monitor
#

bin_PROGRAMS = fapm5

AM_CPPFLAGS = -D_PLAT_LINUX_ -g -O0

fapm5_SOURCES = \
	       monitor.h monitor.cpp main.cpp

fapm5_LDFLAGS=-L../src/.libs/ -lfap5 -lrt
fapm5_CXXFLAGS=-I@srcdir@/../src -pg -pthread

