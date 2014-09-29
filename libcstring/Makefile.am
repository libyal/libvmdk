if HAVE_LOCAL_LIBCSTRING
AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common

noinst_LTLIBRARIES = libcstring.la

libcstring_la_SOURCES = \
	libcstring_definitions.h \
	libcstring_extern.h \
	libcstring_narrow_string.h \
	libcstring_support.c libcstring_support.h \
	libcstring_system_string.h \
	libcstring_types.h \
	libcstring_unused.h \
	libcstring_wide_string.c libcstring_wide_string.h
endif

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

splint:
	@echo "Running splint on libcstring ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(libcstring_la_SOURCES)

