MSVSCPP_FILES = \
	dokan_dll/dokan_dll.vcproj \
	libbfio/libbfio.vcproj \
	libcdata/libcdata.vcproj \
	libcerror/libcerror.vcproj \
	libcfile/libcfile.vcproj \
	libclocale/libclocale.vcproj \
	libcnotify/libcnotify.vcproj \
	libcpath/libcpath.vcproj \
	libcsplit/libcsplit.vcproj \
	libcstring/libcstring.vcproj \
	libcsystem/libcsystem.vcproj \
	libcthreads/libcthreads.vcproj \
	libfcache/libfcache.vcproj \
	libfdata/libfdata.vcproj \
	libfvalue/libfvalue.vcproj \
	libvmdk_dll/libvmdk_dll.vcproj \
	libuna/libuna.vcproj \
	pyvmdk/pyvmdk.vcproj \
	vmdk_test_open_close/vmdk_test_open_close.vcproj \
	vmdk_test_read/vmdk_test_read.vcproj \
	vmdk_test_seek/vmdk_test_seek.vcproj \
	vmdkinfo/vmdkinfo.vcproj \
	vmdkmount/vmdkmount.vcproj \
	zlib_dll/zlib_dll.vcproj \
	libvmdk.sln

SCRIPT_FILES = \
	scripts/vs2008_x64.sh \
	scripts/vs2008_x64_sln.sed \
	scripts/vs2008_x64_vcproj.sed \
	scripts/vs2010_x64.sh \
	scripts/vs2010_x64_sln.sed \
	scripts/vs2010_x64_vcxproj.sed

EXTRA_DIST = \
	$(MSVSCPP_FILES) \
	$(SCRIPT_FILES)

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

