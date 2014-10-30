AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCSTRING_CPPFLAGS@ \
	@LIBCERROR_CPPFLAGS@ \
	@LIBVMDK_DLL_IMPORT@

if HAVE_PYTHON
TESTS_PYVMDK  = \
	test_pyvmdk_open_close.sh
endif

TESTS = \
	test_open_close.sh \
	test_seek.sh \
	test_read.sh \
	test_vmdkinfo.sh \
	$(TESTS_PYVMDK)

check_SCRIPTS = \
	pyvmdk_test_open_close.py \
	test_pyvmdk_open_close.sh \
	test_open_close.sh \
	test_pyvmdk_open_close.sh \
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
	vmdk_test_libvmdk.h \
	vmdk_test_open_close.c \
	vmdk_test_unused.h

vmdk_test_open_close_LDADD = \
	../libvmdk/libvmdk.la \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@

vmdk_test_read_SOURCES = \
	vmdk_test_libcerror.h \
	vmdk_test_libcstring.h \
	vmdk_test_libvmdk.h \
	vmdk_test_read.c \
	vmdk_test_unused.h

vmdk_test_read_LDADD = \
	../libvmdk/libvmdk.la \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@

vmdk_test_seek_SOURCES = \
	vmdk_test_libcerror.h \
	vmdk_test_libcstring.h \
	vmdk_test_libvmdk.h \
	vmdk_test_seek.c \
	vmdk_test_unused.h

vmdk_test_seek_LDADD = \
	../libvmdk/libvmdk.la \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

