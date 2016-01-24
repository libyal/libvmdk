AM_CPPFLAGS = \
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
	@LIBFVALUE_CPPFLAGS@ \
	@LIBCSYSTEM_CPPFLAGS@ \
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
	vmdkoutput.c vmdkoutput.h \
	vmdktools_libbfio.h \
	vmdktools_libcerror.h \
	vmdktools_libclocale.h \
	vmdktools_libcnotify.h \
	vmdktools_libcstring.h \
	vmdktools_libcsystem.h \
	vmdktools_libfvalue.h \
	vmdktools_libuna.h \
	vmdktools_libvmdk.h

vmdkinfo_LDADD = \
	@LIBCSYSTEM_LIBADD@ \
	../libvmdk/libvmdk.la \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@ \
	@LIBINTL@

vmdkmount_SOURCES = \
	mount_handle.c mount_handle.h \
	vmdkmount.c \
	vmdkoutput.c vmdkoutput.h \
	vmdktools_libbfio.h \
	vmdktools_libcdata.h \
	vmdktools_libcerror.h \
	vmdktools_libclocale.h \
	vmdktools_libcnotify.h \
	vmdktools_libcpath.h \
	vmdktools_libcstring.h \
	vmdktools_libcsystem.h \
	vmdktools_libfvalue.h \
	vmdktools_libuna.h \
	vmdktools_libvmdk.h

vmdkmount_LDADD = \
	@LIBFUSE_LIBADD@ \
	@LIBCSYSTEM_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	../libvmdk/libvmdk.la \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@ \
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

