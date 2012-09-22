if HAVE_LOCAL_LIBMFCACHE
AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCSTRING_CPPFLAGS@ \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCNOTIFY_CPPFLAGS@

noinst_LTLIBRARIES = libmfcache.la

libmfcache_la_SOURCES = \
	libmfcache_array_type.c libmfcache_array_type.h \
	libmfcache_cache.c libmfcache_cache.h \
	libmfcache_cache_value.c libmfcache_cache_value.h \
	libmfcache_date_time.h \
	libmfcache_definitions.h \
	libmfcache_error.c libmfcache_error.h \
	libmfcache_extern.h \
	libmfcache_libcerror.h \
	libmfcache_libcnotify.h \
	libmfcache_libcstring.h \
	libmfcache_support.c libmfcache_support.h \
	libmfcache_types.h \
	libmfcache_unused.h
endif

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

splint:
	@echo "Running splint on libmfcache ..." 
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(libmfcache_la_SOURCES) 

