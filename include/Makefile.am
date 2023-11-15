include_HEADERS = \
	libvmdk.h

pkginclude_HEADERS = \
	libvmdk/codepage.h \
	libvmdk/definitions.h \
	libvmdk/error.h \
	libvmdk/extern.h \
	libvmdk/features.h \
	libvmdk/types.h

EXTRA_DIST = \
	libvmdk.h.in \
	libvmdk/definitions.h.in \
	libvmdk/features.h.in \
	libvmdk/types.h.in

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	-rm -f libvmdk.h
	-rm -f libvmdk/definitions.h
	-rm -f libvmdk/features.h
	-rm -f libvmdk/types.h
	-rm -f Makefile

