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
	@LIBVHDI_DLL_IMPORT@

AM_LDFLAGS = @STATIC_LDFLAGS@

bin_PROGRAMS = vmdkinfo

vmdkinfo_SOURCES = \
	vmdkinfo.c \
	vmdkoutput.c vmdkoutput.h

vmdkinfo_LDADD  = \
	@LIBCSYSTEM_LIBADD@ \
	../libvhdi/libvhdi.la \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@ \
	@LIBINTL@

distclean: clean
	/bin/rm -f Makefile

splint:
	@echo "Running splint on vmdkinfo ..." 
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(vmdkinfo_SOURCES) 

