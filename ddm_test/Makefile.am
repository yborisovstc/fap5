bin_PROGRAMS = ut-fap5srv-lib

AM_CPPFLAGS = -D_PLAT_LINUX_ -g -O0

ut_fap5srv_lib_SOURCES = \
		       ut_main.cpp \
		       ut_base.cpp

#ut_fap5srv_lib_LDFLAGS=-L../client/.libs/ -lfap5client -L../server/.libs/ -lfap5srv -L../dmas/.libs -lfap5da -lcppunit -lrt
ut_fap5srv_lib_LDFLAGS=-L../src/.libs/ -lfap5 -L../client/.libs/ -lfap5client -L../dmas/.libs -lfap5da -lcppunit -lrt
ut_fap5srv_lib_CXXFLAGS=-I@srcdir@/../dmas

check: test_fap5srv_lib

test_fap5srv_lib:
	 ./ut-fap5srv-lib


