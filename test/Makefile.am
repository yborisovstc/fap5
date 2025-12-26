bin_PROGRAMS = ut-fap5-lib

AM_CPPFLAGS = -D_PLAT_LINUX_

ut_fap5_lib_SOURCES = \
		       ut_fap5_main.cpp \
		       ut_desas.cpp \
		       ut_dessdc.cpp \
		       ut_desctx.cpp \
		       ut_adp.cpp \
		       ut_des.cpp \
		       ut_des2.cpp \
		       ut_node.cpp \
		       ut_base.h \
		       ut_base.cpp

ut_fap5_lib_LDFLAGS=-L../src/.libs/ -lfap5  -lcppunit -lrt
ut_fap5_lib_CXXFLAGS=-I@srcdir@/../src

check: test_fap5_lib

test_fap5_lib:
	 ./ut-fap5-lib


