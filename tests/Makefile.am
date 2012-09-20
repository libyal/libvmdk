AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCSTRING_CPPFLAGS@ \
	@LIBCERROR_CPPFLAGS@ \
	@LIBVHDI_DLL_IMPORT@

TESTS = \
	test_seek.sh \
	test_read.sh

check_SCRIPTS = \
	test_read.sh \
	test_seek.sh

EXTRA_DIST = \
	$(check_SCRIPTS)

check_PROGRAMS = \
	vhdi_test_read \
	vhdi_test_seek

vhdi_test_read_SOURCES = \
	vhdi_test_libcerror.h \
	vhdi_test_libcstring.h \
	vhdi_test_libvhdi.h \
	vhdi_test_read.c

vhdi_test_read_LDADD = \
	@LIBCSTRING_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	../libvhdi/libvhdi.la

vhdi_test_seek_SOURCES = \
	vhdi_test_libcerror.h \
	vhdi_test_libcstring.h \
	vhdi_test_libvhdi.h \
	vhdi_test_seek.c

vhdi_test_seek_LDADD = \
	@LIBCSTRING_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	../libvhdi/libvhdi.la

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

