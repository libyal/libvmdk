AM_CPPFLAGS = \
	-DLOCALEDIR=\"$(datadir)/locale\" \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCSTRING_CPPFLAGS@ \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCDATA_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBCNOTIFY_CPPFLAGS@ \
	@LIBCSPLIT_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBCFILE_CPPFLAGS@ \
	@LIBCPATH_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@ \
	@LIBFCACHE_CPPFLAGS@ \
	@LIBFDATA_CPPFLAGS@ \
	@LIBFVALUE_CPPFLAGS@

lib_LTLIBRARIES = libvmdk.la

libvmdk_la_SOURCES = \
	libvmdk.c \
	libvmdk_debug.c libvmdk_debug.h \
	libvmdk_definitions.h \
	libvmdk_descriptor_file.c libvmdk_descriptor_file.h \
	libvmdk_error.c libvmdk_error.h \
	libvmdk_extern.h \
	libvmdk_grain_offset.h \
	libvmdk_handle.c libvmdk_handle.h \
	libvmdk_io_handle.c libvmdk_io_handle.h \
	libvmdk_libbfio.h \
	libvmdk_libcdata.h \
	libvmdk_libcerror.h \
	libvmdk_libcnotify.h \
	libvmdk_libcstring.h \
	libvmdk_libfvalue.h \
	libvmdk_libuna.h \
	libvmdk_notify.c libvmdk_notify.h \
	libvmdk_offset_table.c libvmdk_offset_table.h \
	libvmdk_segment_file.c libvmdk_segment_file.h \
	libvmdk_segment_file_handle.c libvmdk_segment_file_handle.h \
	libvmdk_segment_table.c libvmdk_segment_table.h \
	libvmdk_support.c libvmdk_support.h \
	libvmdk_unused.h \
	cowd_sparse_file_header.h \
	vmdk_sparse_file_header.h

libvmdk_la_LIBADD = \
	@LIBCSTRING_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBBFIO_LIBADD@ \
	@LIBFCACHE_LIBADD@ \
	@LIBFDATA_LIBADD@ \
	@LIBFVALUE_LIBADD@

libvmdk_la_LDFLAGS = -no-undefined -version-info 1:0:0

EXTRA_DIST = \
	libvmdk_definitions.h.in \
	libvmdk.rc \
	libvmdk.rc.in

distclean: clean
	/bin/rm -f libvmdk_definitions.h
	/bin/rm -f libvmdk.rc
	/bin/rm -f Makefile

splint:
	@echo "Running splint on libvmdk ..." 
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(libvmdk_la_SOURCES) 

