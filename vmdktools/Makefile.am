AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	-I$(top_srcdir)/liberror \
	-I$(top_srcdir)/libsystem \
	@LIBUNA_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@ \
	@MEMWATCH_CPPFLAGS@ \
	@LIBVMDK_DLL_IMPORT@

AM_LDFLAGS = @STATIC_LDFLAGS@

bin_PROGRAMS = vmdkinfo

vmdkinfo_SOURCES = \
	filetime.c filetime.h \
	guid.c guid.h \
	vmdkinfo.c \
	vmdkoutput.c vmdkoutput.h

vmdkinfo_LDADD  = \
	../liberror/liberror.la \
	../libvmdk/libvmdk.la \
	../libsystem/libsystem.la \
	@MEMWATCH_LIBADD@

distclean: clean
	/bin/rm -f Makefile

splint:
	@echo "Running splint on vmdkinfo ..." 
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(vmdkinfo_SOURCES) 

