AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCSTRING_CPPFLAGS@ \
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
	@LIBCSYSTEM_CPPFLAGS@ \
	@PTHREAD_CPPFLAGS@ \
	@LIBVMDK_DLL_IMPORT@

if HAVE_PYTHON_TESTS
TESTS_PYVMDK  = \
	test_pyvmdk_open_close.sh \
	test_pyvmdk_seek.sh
endif

TESTS = \
	test_open_close.sh \
	test_seek.sh \
	test_read.sh \
	test_vmdkinfo.sh \
	$(TESTS_PYVMDK)

check_SCRIPTS = \
	pyvmdk_test_open_close.py \
	pyvmdk_test_seek.py \
	test_open_close.sh \
	test_pyvmdk_open_close.sh \
	test_pyvmdk_seek.sh \
	test_read.sh \
	test_runner.sh \
	test_seek.sh \
	test_vmdkinfo.sh

EXTRA_DIST = \
	$(check_SCRIPTS)

check_PROGRAMS = \
	vmdk_test_open_close \
	vmdk_test_read \
	vmdk_test_seek

vmdk_test_open_close_SOURCES = \
	vmdk_test_libcerror.h \
	vmdk_test_libcstring.h \
	vmdk_test_libcsystem.h \
	vmdk_test_libcthreads.h \
	vmdk_test_libvmdk.h \
	vmdk_test_open_close.c \
	vmdk_test_unused.h

vmdk_test_open_close_LDADD = \
	@LIBCSYSTEM_LIBADD@ \
	../libvmdk/libvmdk.la \
	@LIBCTHREADS_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@ \
	@PTHREAD_LIBADD@

vmdk_test_read_SOURCES = \
	vmdk_test_libcerror.h \
	vmdk_test_libcstring.h \
	vmdk_test_libcsystem.h \
	vmdk_test_libcthreads.h \
	vmdk_test_libvmdk.h \
	vmdk_test_read.c \
	vmdk_test_unused.h

vmdk_test_read_LDADD = \
	@LIBCSYSTEM_LIBADD@ \
	../libvmdk/libvmdk.la \
	@LIBCTHREADS_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@ \
	@PTHREAD_LIBADD@

vmdk_test_seek_SOURCES = \
	vmdk_test_libcerror.h \
	vmdk_test_libcstring.h \
	vmdk_test_libcsystem.h \
	vmdk_test_libcthreads.h \
	vmdk_test_libvmdk.h \
	vmdk_test_seek.c \
	vmdk_test_unused.h

vmdk_test_seek_LDADD = \
	@LIBCSYSTEM_LIBADD@ \
	../libvmdk/libvmdk.la \
	@LIBCTHREADS_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@ \
	@PTHREAD_LIBADD@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

