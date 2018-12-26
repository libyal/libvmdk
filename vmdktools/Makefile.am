AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
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
	@LIBFVALUE_CPPFLAGS@ \
	@LIBFUSE_CPPFLAGS@ \
	@LIBVMDK_DLL_IMPORT@

AM_LDFLAGS = @STATIC_LDFLAGS@

bin_PROGRAMS = \
	vmdkinfo \
	vmdkmount

vmdkinfo_SOURCES = \
	byte_size_string.c byte_size_string.h \
	info_handle.c info_handle.h \
	vmdkinfo.c \
	vmdktools_getopt.c vmdktools_getopt.h \
	vmdktools_i18n.h \
	vmdktools_libbfio.h \
	vmdktools_libcerror.h \
	vmdktools_libclocale.h \
	vmdktools_libcnotify.h \
	vmdktools_libfvalue.h \
	vmdktools_libuna.h \
	vmdktools_libvmdk.h \
	vmdktools_output.c vmdktools_output.h \
	vmdktools_signal.c vmdktools_signal.h \
	vmdktools_unused.h

vmdkinfo_LDADD = \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	../libvmdk/libvmdk.la \
	@LIBCERROR_LIBADD@ \
	@LIBINTL@

vmdkmount_SOURCES = \
	mount_dokan.c mount_dokan.h \
	mount_file_entry.c mount_file_entry.h \
	mount_file_system.c mount_file_system.h \
	mount_fuse.c mount_fuse.h \
	mount_handle.c mount_handle.h \
	vmdkmount.c \
	vmdktools_getopt.c vmdktools_getopt.h \
	vmdktools_i18n.h \
	vmdktools_libbfio.h \
	vmdktools_libcdata.h \
	vmdktools_libcerror.h \
	vmdktools_libclocale.h \
	vmdktools_libcnotify.h \
	vmdktools_libcpath.h \
	vmdktools_libfvalue.h \
	vmdktools_libuna.h \
	vmdktools_libvmdk.h \
	vmdktools_output.c vmdktools_output.h \
	vmdktools_signal.c vmdktools_signal.h \
	vmdktools_unused.h

vmdkmount_LDADD = \
	@LIBFUSE_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libvmdk/libvmdk.la \
	@LIBCERROR_LIBADD@ \
	@LIBINTL@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

splint:
	@echo "Running splint on vmdkinfo ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(vmdkinfo_SOURCES)
	@echo "Running splint on vmdkmount ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(vmdkmount_SOURCES)

