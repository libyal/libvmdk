AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	-I$(top_srcdir)/libcstring \
	-I$(top_srcdir)/liberror \
	-I$(top_srcdir)/libnotify \
	@LIBUNA_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@ \
	@MEMWATCH_CPPFLAGS@

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
	libvmdk_libuna.h \
	libvmdk_notify.c libvmdk_notify.h \
	libvmdk_offset_table.c libvmdk_offset_table.h \
	libvmdk_support.c libvmdk_support.h \
	libvmdk_values_table.c libvmdk_values_table.h \
	cowd_sparse_file_header.h \
	vmdk_sparse_file_header.h

libvmdk_la_LIBADD = \
	../libcstring/libcstring.la \
	../liberror/liberror.la \
	../libnotify/libnotify.la \
	@LIBUNA_LIBADD@ \
	@LIBBFIO_LIBADD@

libvmdk_la_LDFLAGS = -no-undefined -version-info 1:0:0

EXTRA_DIST = \
	libvmdk_definitions.h.in

distclean: clean
	/bin/rm -f Makefile

splint:
	@echo "Running splint on libvmdk ..." 
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(libvmdk_la_SOURCES) 

