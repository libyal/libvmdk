if HAVE_LOCAL_LIBCSYSTEM
AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCSTRING_CPPFLAGS@ \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBCNOTIFY_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@

noinst_LTLIBRARIES = libcsystem.la

libcsystem_la_SOURCES = \
	libcsystem_definitions.h \
	libcsystem_find.h \
	libcsystem_file_io.h \
	libcsystem_file_stream.h \
	libcsystem_getopt.c libcsystem_getopt.h \
	libcsystem_glob.c libcsystem_glob.h \
	libcsystem_i18n.h \
	libcsystem_libcstring.h \
	libcsystem_libcerror.h \
	libcsystem_libclocale.h \
	libcsystem_libcnotify.h \
	libcsystem_libuna.h \
	libcsystem_signal.c libcsystem_signal.h \
	libcsystem_string.c libcsystem_string.h \
	libcsystem_support.c libcsystem_support.h \
	libcsystem_unused.h

libcsystem_la_LIBADD = \
	@LIBCSTRING_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBUNA_LIBADD@
endif

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

splint:
	@echo "Running splint on libcsystem ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(libcsystem_la_SOURCES)

