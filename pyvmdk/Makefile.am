if HAVE_PYTHON
AM_CFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCDATA_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBCSPLIT_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBCFILE_CPPFLAGS@ \
	@LIBCPATH_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@ \
	@LIBVMDK_DLL_IMPORT@

pyexec_LTLIBRARIES = pyvmdk.la

pyvmdk_la_SOURCES = \
	pyvmdk.c pyvmdk.h \
	pyvmdk_disk_types.c pyvmdk_disk_types.h \
	pyvmdk_error.c pyvmdk_error.h \
	pyvmdk_extent_descriptor.c pyvmdk_extent_descriptor.h \
	pyvmdk_extent_descriptors.c pyvmdk_extent_descriptors.h \
	pyvmdk_extent_types.c pyvmdk_extent_types.h \
	pyvmdk_file_object_io_handle.c pyvmdk_file_object_io_handle.h \
	pyvmdk_file_objects_io_pool.c pyvmdk_file_objects_io_pool.h \
	pyvmdk_handle.c pyvmdk_handle.h \
	pyvmdk_integer.c pyvmdk_integer.h \
	pyvmdk_libbfio.h \
	pyvmdk_libcerror.h \
	pyvmdk_libclocale.h \
	pyvmdk_libvmdk.h \
	pyvmdk_python.h \
	pyvmdk_unused.h

pyvmdk_la_LIBADD = \
	@LIBCERROR_LIBADD@ \
	../libvmdk/libvmdk.la \
	@LIBCDATA_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBBFIO_LIBADD@

pyvmdk_la_CPPFLAGS = $(PYTHON_CPPFLAGS)
pyvmdk_la_LDFLAGS  = -module -avoid-version $(PYTHON_LDFLAGS)

endif

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	-rm -f Makefile

