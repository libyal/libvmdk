if HAVE_LOCAL_LIBMFDATA
AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCSTRING_CPPFLAGS@ \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCDATA_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBCNOTIFY_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBCFILE_CPPFLAGS@ \
	@LIBCPATH_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@ \
	@LIBMFCACHE_CPPFLAGS@

noinst_LTLIBRARIES = libmfdata.la

libmfdata_la_SOURCES = \
	libmfdata_date_time.h \
	libmfdata_definitions.h \
	libmfdata_error.c libmfdata_error.h \
	libmfdata_extern.h \
	libmfdata_file.c libmfdata_file.h \
	libmfdata_file_list.c libmfdata_file_list.h \
	libmfdata_group.c libmfdata_group.h \
	libmfdata_inline.h \
	libmfdata_libbfio.h \
	libmfdata_libcdata.h \
	libmfdata_libcerror.h \
	libmfdata_libcnotify.h \
	libmfdata_libcstring.h \
	libmfdata_libmfcache.h \
	libmfdata_list.c libmfdata_list.h \
	libmfdata_list_element.c libmfdata_list_element.h \
	libmfdata_range.c libmfdata_range.h \
	libmfdata_segment.c libmfdata_segment.h \
	libmfdata_segment_table.c libmfdata_segment_table.h \
	libmfdata_notify.c libmfdata_notify.h \
	libmfdata_support.c libmfdata_support.h \
	libmfdata_types.h \
	libmfdata_unused.h
endif

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

splint:
	@echo "Running splint on libmfdata ..." 
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(libmfdata_la_SOURCES) 

