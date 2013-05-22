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
	$(TESTS_PYVMDK)

check_SCRIPTS = \
	test_pyvmdk_open_close.sh \
	test_open_close.sh \
	test_pyvmdk_open_close.sh \
	test_read.sh \
	test_seek.sh

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
	@LIBCSTRING_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	../libvmdk/libvmdk.la

vmdk_test_read_SOURCES = \
	vmdk_test_libcerror.h \
	vmdk_test_libcstring.h \
	vmdk_test_libvmdk.h \
	vmdk_test_read.c \
	vmdk_test_unused.h

vmdk_test_read_LDADD = \
	@LIBCSTRING_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	../libvmdk/libvmdk.la

vmdk_test_seek_SOURCES = \
	vmdk_test_libcerror.h \
	vmdk_test_libcstring.h \
	vmdk_test_libvmdk.h \
	vmdk_test_seek.c \
	vmdk_test_unused.h

vmdk_test_seek_LDADD = \
	@LIBCSTRING_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	../libvmdk/libvmdk.la

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

