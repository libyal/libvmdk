AM_CPPFLAGS = \
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
	@LIBCSYSTEM_CPPFLAGS@ \
	@ZLIB_CPPFLAGS@ \
	@PTHREAD_CPPFLAGS@ \
	@LIBVMDK_DLL_IMPORT@

if HAVE_PYTHON_TESTS
TESTS_PYVMDK = \
	test_python_functions.sh
endif

TESTS = \
	test_api_functions.sh \
	test_api_types.sh \
	test_vmdkinfo.sh \
	$(TESTS_PYVMDK)

check_SCRIPTS = \
	pyvmdk_test_get_version.py \
	pyvmdk_test_open_close.py \
	pyvmdk_test_read.py \
	pyvmdk_test_seek.py \
	test_api_functions.sh \
	test_api_types.sh \
	test_python_functions.sh \
	test_runner.sh \
	test_vmdkinfo.sh

EXTRA_DIST = \
	$(check_SCRIPTS)

check_PROGRAMS = \
	vmdk_test_descriptor_file \
	vmdk_test_error \
	vmdk_test_extent_descriptor \
	vmdk_test_extent_file \
	vmdk_test_extent_table \
	vmdk_test_grain_data \
	vmdk_test_grain_group \
	vmdk_test_grain_table \
	vmdk_test_handle \
	vmdk_test_io_handle \
	vmdk_test_notify \
	vmdk_test_read \
	vmdk_test_seek \
	vmdk_test_support

vmdk_test_descriptor_file_SOURCES = \
	vmdk_test_descriptor_file.c \
	vmdk_test_libcerror.h \
	vmdk_test_libvmdk.h \
	vmdk_test_macros.h \
	vmdk_test_memory.c vmdk_test_memory.h \
	vmdk_test_unused.h

vmdk_test_descriptor_file_LDADD = \
	../libvmdk/libvmdk.la \
	@LIBCERROR_LIBADD@

vmdk_test_error_SOURCES = \
	vmdk_test_error.c \
	vmdk_test_libvmdk.h \
	vmdk_test_macros.h \
	vmdk_test_unused.h

vmdk_test_error_LDADD = \
	../libvmdk/libvmdk.la

vmdk_test_extent_descriptor_SOURCES = \
	vmdk_test_extent_descriptor.c \
	vmdk_test_libcerror.h \
	vmdk_test_libvmdk.h \
	vmdk_test_macros.h \
	vmdk_test_memory.c vmdk_test_memory.h \
	vmdk_test_unused.h

vmdk_test_extent_descriptor_LDADD = \
	../libvmdk/libvmdk.la \
	@LIBCERROR_LIBADD@

vmdk_test_extent_file_SOURCES = \
	vmdk_test_extent_file.c \
	vmdk_test_libcerror.h \
	vmdk_test_libvmdk.h \
	vmdk_test_macros.h \
	vmdk_test_memory.c vmdk_test_memory.h \
	vmdk_test_unused.h

vmdk_test_extent_file_LDADD = \
	../libvmdk/libvmdk.la \
	@LIBCERROR_LIBADD@

vmdk_test_extent_table_SOURCES = \
	vmdk_test_extent_table.c \
	vmdk_test_libcerror.h \
	vmdk_test_libvmdk.h \
	vmdk_test_macros.h \
	vmdk_test_memory.c vmdk_test_memory.h \
	vmdk_test_unused.h

vmdk_test_extent_table_LDADD = \
	../libvmdk/libvmdk.la \
	@LIBCERROR_LIBADD@

vmdk_test_grain_data_SOURCES = \
	vmdk_test_grain_data.c \
	vmdk_test_libcerror.h \
	vmdk_test_libvmdk.h \
	vmdk_test_macros.h \
	vmdk_test_memory.c vmdk_test_memory.h \
	vmdk_test_unused.h

vmdk_test_grain_data_LDADD = \
	../libvmdk/libvmdk.la \
	@LIBCERROR_LIBADD@

vmdk_test_grain_group_SOURCES = \
	vmdk_test_grain_group.c \
	vmdk_test_libcerror.h \
	vmdk_test_libvmdk.h \
	vmdk_test_macros.h \
	vmdk_test_memory.c vmdk_test_memory.h \
	vmdk_test_unused.h

vmdk_test_grain_group_LDADD = \
	../libvmdk/libvmdk.la \
	@LIBCERROR_LIBADD@

vmdk_test_grain_table_SOURCES = \
	vmdk_test_grain_table.c \
	vmdk_test_libcerror.h \
	vmdk_test_libvmdk.h \
	vmdk_test_macros.h \
	vmdk_test_memory.c vmdk_test_memory.h \
	vmdk_test_unused.h

vmdk_test_grain_table_LDADD = \
	../libvmdk/libvmdk.la \
	@LIBCERROR_LIBADD@

vmdk_test_handle_SOURCES = \
	vmdk_test_handle.c \
	vmdk_test_libcerror.h \
	vmdk_test_libclocale.h \
	vmdk_test_libcsystem.h \
	vmdk_test_libuna.h \
	vmdk_test_libvmdk.h \
	vmdk_test_macros.h \
	vmdk_test_memory.c vmdk_test_memory.h

vmdk_test_handle_LDADD = \
	@LIBCSYSTEM_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libvmdk/libvmdk.la \
	@LIBCTHREADS_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@PTHREAD_LIBADD@

vmdk_test_io_handle_SOURCES = \
	vmdk_test_io_handle.c \
	vmdk_test_libcerror.h \
	vmdk_test_libvmdk.h \
	vmdk_test_macros.h \
	vmdk_test_memory.c vmdk_test_memory.h \
	vmdk_test_unused.h

vmdk_test_io_handle_LDADD = \
	../libvmdk/libvmdk.la \
	@LIBCERROR_LIBADD@

vmdk_test_notify_SOURCES = \
	vmdk_test_libvmdk.h \
	vmdk_test_macros.h \
	vmdk_test_notify.c \
	vmdk_test_unused.h

vmdk_test_notify_LDADD = \
	../libvmdk/libvmdk.la

vmdk_test_read_SOURCES = \
	vmdk_test_libcerror.h \
	vmdk_test_libcsystem.h \
	vmdk_test_libcthreads.h \
	vmdk_test_libvmdk.h \
	vmdk_test_read.c \
	vmdk_test_unused.h

vmdk_test_read_LDADD = \
	@LIBCSYSTEM_LIBADD@ \
	../libvmdk/libvmdk.la \
	@LIBCTHREADS_LIBADD@ \
	@LIBCERROR_LIBADD@
	@PTHREAD_LIBADD@

vmdk_test_seek_SOURCES = \
	vmdk_test_libcerror.h \
	vmdk_test_libcsystem.h \
	vmdk_test_libcthreads.h \
	vmdk_test_libvmdk.h \
	vmdk_test_seek.c \
	vmdk_test_unused.h

vmdk_test_seek_LDADD = \
	@LIBCSYSTEM_LIBADD@ \
	../libvmdk/libvmdk.la \
	@LIBCTHREADS_LIBADD@ \
	@LIBCERROR_LIBADD@
	@PTHREAD_LIBADD@

vmdk_test_support_SOURCES = \
	vmdk_test_libcerror.h \
	vmdk_test_libclocale.h \
	vmdk_test_libcsystem.h \
	vmdk_test_libuna.h \
	vmdk_test_libvmdk.h \
	vmdk_test_macros.h \
	vmdk_test_support.c

vmdk_test_support_LDADD = \
	@LIBCSYSTEM_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libvmdk/libvmdk.la \
	@LIBCERROR_LIBADD@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

