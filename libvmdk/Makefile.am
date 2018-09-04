AM_CPPFLAGS = \
	-DLOCALEDIR=\"$(datadir)/locale\" \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCTHREADS_CPPFLAGS@ \
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
	@LIBFVALUE_CPPFLAGS@ \
	@ZLIB_CPPFLAGS@ \
	@PTHREAD_CPPFLAGS@ \
	@LIBVMDK_DLL_EXPORT@

lib_LTLIBRARIES = libvmdk.la

libvmdk_la_SOURCES = \
	libvmdk.c \
	libvmdk_compression.c libvmdk_compression.h \
	libvmdk_debug.c libvmdk_debug.h \
	libvmdk_deflate.c libvmdk_deflate.h \
	libvmdk_definitions.h \
	libvmdk_descriptor_file.c libvmdk_descriptor_file.h \
	libvmdk_error.c libvmdk_error.h \
	libvmdk_extent_descriptor.c libvmdk_extent_descriptor.h \
	libvmdk_extent_file.c libvmdk_extent_file.h \
	libvmdk_extent_table.c libvmdk_extent_table.h \
	libvmdk_extern.h \
	libvmdk_grain_data.c libvmdk_grain_data.h \
	libvmdk_grain_group.c libvmdk_grain_group.h \
	libvmdk_grain_table.c libvmdk_grain_table.h \
	libvmdk_handle.c libvmdk_handle.h \
	libvmdk_io_handle.c libvmdk_io_handle.h \
	libvmdk_libbfio.h \
	libvmdk_libcdata.h \
	libvmdk_libcerror.h \
	libvmdk_libclocale.h \
	libvmdk_libcnotify.h \
	libvmdk_libcpath.h \
	libvmdk_libcsplit.h \
	libvmdk_libcthreads.h \
	libvmdk_libfcache.h \
	libvmdk_libfdata.h \
	libvmdk_libfvalue.h \
	libvmdk_libuna.h \
	libvmdk_notify.c libvmdk_notify.h \
	libvmdk_support.c libvmdk_support.h \
	libvmdk_system_string.c libvmdk_system_string.h \
	libvmdk_types.h \
	libvmdk_unused.h \
	cowd_sparse_file_header.h \
	vmdk_sparse_file_header.h

libvmdk_la_LIBADD = \
	@LIBCERROR_LIBADD@ \
	@LIBCTHREADS_LIBADD@ \
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
	@LIBFVALUE_LIBADD@ \
	@ZLIB_LIBADD@ \
	@PTHREAD_LIBADD@

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

