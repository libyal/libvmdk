AUTOMAKE_OPTIONS = subdir-objects

AM_CPPFLAGS = \
	-I../include -I$(top_srcdir)/include \
	-I../common -I$(top_srcdir)/common \
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
	@ZLIB_CPPFLAGS@ \
	@PTHREAD_CPPFLAGS@ \
	@LIBVMDK_DLL_IMPORT@

BUILT_SOURCES = package.m4

check_AUTOTESTS = \
	test_library \
	test_manpages \
	test_tools

if HAVE_PYTHON_TESTS
check_AUTOTESTS += test_python_module
endif

check_SCRIPTS = \
	pyvmdk_test_handle.py \
	pyvmdk_test_support.py

check_PROGRAMS = \
	vmdk_test_bit_stream \
	vmdk_test_compression \
	vmdk_test_deflate \
	vmdk_test_descriptor_file \
	vmdk_test_error \
	vmdk_test_extent_descriptor \
	vmdk_test_extent_file \
	vmdk_test_extent_table \
	vmdk_test_extent_values \
	vmdk_test_grain_data \
	vmdk_test_grain_group \
	vmdk_test_grain_table \
	vmdk_test_handle \
	vmdk_test_huffman_tree \
	vmdk_test_io_handle \
	vmdk_test_notify \
	vmdk_test_support \
	vmdk_test_system_string \
	vmdk_test_tools_info_handle \
	vmdk_test_tools_output \
	vmdk_test_tools_signal

vmdk_test_bit_stream_SOURCES = \
	vmdk_test_bit_stream.c \
	vmdk_test_libcerror.h \
	vmdk_test_libvmdk.h \
	vmdk_test_macros.h \
	vmdk_test_memory.c vmdk_test_memory.h \
	vmdk_test_unused.h

vmdk_test_bit_stream_LDADD = \
	../libvmdk/libvmdk.la \
	@LIBCERROR_LIBADD@

vmdk_test_compression_SOURCES = \
	vmdk_test_compression.c \
	vmdk_test_libcerror.h \
	vmdk_test_libvmdk.h \
	vmdk_test_macros.h \
	vmdk_test_memory.c vmdk_test_memory.h \
	vmdk_test_unused.h

vmdk_test_compression_LDADD = \
	../libvmdk/libvmdk.la \
	@LIBCERROR_LIBADD@

vmdk_test_deflate_SOURCES = \
	vmdk_test_deflate.c \
	vmdk_test_libcerror.h \
	vmdk_test_libcnotify.h \
	vmdk_test_libvmdk.h \
	vmdk_test_macros.h \
	vmdk_test_unused.h

vmdk_test_deflate_LDADD = \
	@LIBCNOTIFY_LIBADD@ \
	../libvmdk/libvmdk.la \
	@LIBCERROR_LIBADD@

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
	vmdk_test_libcpath.h \
	vmdk_test_libvmdk.h \
	vmdk_test_macros.h \
	vmdk_test_memory.c vmdk_test_memory.h \
	vmdk_test_unused.h

vmdk_test_extent_table_LDADD = \
	../libvmdk/libvmdk.la \
	@LIBCERROR_LIBADD@

vmdk_test_extent_values_SOURCES = \
	vmdk_test_extent_values.c \
	vmdk_test_libcerror.h \
	vmdk_test_libvmdk.h \
	vmdk_test_macros.h \
	vmdk_test_memory.c vmdk_test_memory.h \
	vmdk_test_unused.h

vmdk_test_extent_values_LDADD = \
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
	vmdk_test_functions.c vmdk_test_functions.h \
	vmdk_test_getopt.c vmdk_test_getopt.h \
	vmdk_test_handle.c \
	vmdk_test_libbfio.h \
	vmdk_test_libcerror.h \
	vmdk_test_libclocale.h \
	vmdk_test_libcnotify.h \
	vmdk_test_libuna.h \
	vmdk_test_libvmdk.h \
	vmdk_test_macros.h \
	vmdk_test_memory.c vmdk_test_memory.h \
	vmdk_test_unused.h

vmdk_test_handle_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libvmdk/libvmdk.la \
	@LIBCTHREADS_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@PTHREAD_LIBADD@

vmdk_test_huffman_tree_SOURCES = \
	vmdk_test_huffman_tree.c \
	vmdk_test_libcerror.h \
	vmdk_test_libvmdk.h \
	vmdk_test_macros.h \
	vmdk_test_memory.c vmdk_test_memory.h \
	vmdk_test_unused.h

vmdk_test_huffman_tree_LDADD = \
	../libvmdk/libvmdk.la \
	@LIBCERROR_LIBADD@

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
	vmdk_test_libcerror.h \
	vmdk_test_libvmdk.h \
	vmdk_test_macros.h \
	vmdk_test_notify.c \
	vmdk_test_unused.h

vmdk_test_notify_LDADD = \
	../libvmdk/libvmdk.la \
	@LIBCERROR_LIBADD@

vmdk_test_support_SOURCES = \
	vmdk_test_functions.c vmdk_test_functions.h \
	vmdk_test_getopt.c vmdk_test_getopt.h \
	vmdk_test_libbfio.h \
	vmdk_test_libcerror.h \
	vmdk_test_libclocale.h \
	vmdk_test_libcnotify.h \
	vmdk_test_libuna.h \
	vmdk_test_libvmdk.h \
	vmdk_test_macros.h \
	vmdk_test_memory.c vmdk_test_memory.h \
	vmdk_test_support.c \
	vmdk_test_unused.h

vmdk_test_support_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libvmdk/libvmdk.la \
	@LIBCERROR_LIBADD@

vmdk_test_system_string_SOURCES = \
	vmdk_test_libcerror.h \
	vmdk_test_libvmdk.h \
	vmdk_test_macros.h \
	vmdk_test_system_string.c \
	vmdk_test_unused.h

vmdk_test_system_string_LDADD = \
	../libvmdk/libvmdk.la \
	@LIBCERROR_LIBADD@

vmdk_test_tools_info_handle_SOURCES = \
	../vmdktools/byte_size_string.c ../vmdktools/byte_size_string.h \
	../vmdktools/info_handle.c ../vmdktools/info_handle.h \
	vmdk_test_libcerror.h \
	vmdk_test_macros.h \
	vmdk_test_memory.c vmdk_test_memory.h \
	vmdk_test_tools_info_handle.c \
	vmdk_test_unused.h

vmdk_test_tools_info_handle_LDADD = \
	@LIBCLOCALE_LIBADD@ \
	../libvmdk/libvmdk.la \
	@LIBCERROR_LIBADD@

vmdk_test_tools_output_SOURCES = \
	../vmdktools/vmdktools_output.c ../vmdktools/vmdktools_output.h \
	vmdk_test_libcerror.h \
	vmdk_test_macros.h \
	vmdk_test_tools_output.c \
	vmdk_test_unused.h

vmdk_test_tools_output_LDADD = \
	../libvmdk/libvmdk.la \
	@LIBCERROR_LIBADD@ \
	@LIBINTL@

vmdk_test_tools_signal_SOURCES = \
	../vmdktools/vmdktools_signal.c ../vmdktools/vmdktools_signal.h \
	vmdk_test_libcerror.h \
	vmdk_test_macros.h \
	vmdk_test_tools_signal.c \
	vmdk_test_unused.h

vmdk_test_tools_signal_LDADD = \
	../libvmdk/libvmdk.la \
	@LIBCERROR_LIBADD@

AUTOM4TE = autom4te
AUTOTEST = $(AUTOM4TE) --language=autotest

SUFFIXES = .at

.at:
	$(AM_V_GEN)rm -f $@ $@.tmp; \
	$(AUTOTEST) -I '$(srcdir)' -o $@.tmp $< && \
	mv $@.tmp $@

atconfig: $(top_builddir)/config.status
	cd $(top_builddir) && $(SHELL) ./config.status tests/$@

check-build: $(check_PROGRAMS)

check-local: $(check_AUTOTESTS)
	@fail=0; \
	for test_suite in $(check_AUTOTESTS); do \
		$(AM_TESTS_ENVIRONMENT) $(SHELL) "./$$test_suite" $(TESTSUITEFLAGS) || fail=1; \
		echo ""; \
	done; \
	exit $$fail

clean-local:
	for test_suite in $(check_AUTOTESTS); do \
		test ! -f "$$test_suite" || $(SHELL) "./$$test_suite" --clean; \
	done

package.m4: $(top_srcdir)/configure.ac
	$(AM_V_GEN):; \
	{ \
	  echo 'm4_define([AT_PACKAGE_NAME], [$(PACKAGE_NAME)])' ; \
	  echo 'm4_define([AT_PACKAGE_TARNAME], [$(PACKAGE_TARNAME)])' ; \
	  echo 'm4_define([AT_PACKAGE_VERSION], [$(PACKAGE_VERSION)])' ; \
	  echo 'm4_define([AT_PACKAGE_STRING], [$(PACKAGE_STRING)])' ; \
	  echo 'm4_define([AT_PACKAGE_BUGREPORT], [$(PACKAGE_BUGREPORT)])' ; \
	} > $@

test_inputs_libvmdk.at: generate_test_inputs.sh
	$(SHELL) $(srcdir)/generate_test_inputs.sh $(PACKAGE_NAME) libvmdk

test_inputs_pyvmdk.at: generate_test_inputs.sh
	$(SHELL) $(srcdir)/generate_test_inputs.sh $(PACKAGE_NAME) pyvmdk

test_inputs_vmdkinfo.at: generate_test_inputs.sh
	$(SHELL) $(srcdir)/generate_test_inputs.sh $(PACKAGE_NAME) vmdkinfo

test_library: \
	package.m4 \
	test_inputs_libvmdk.at \
	test_library.at \
	test_macros.at

test_manpages: \
	package.m4 \
	test_manpages.at

test_python_module: \
	package.m4 \
	test_inputs_pyvmdk.at \
	test_python_module.at

test_tools: \
	package.m4 \
	test_inputs_vmdkinfo.at \
	test_macros.at \
	test_tools.at

EXTRA_DIST = \
	$(check_AUTOTESTS:=.at) \
	$(check_SCRIPTS) \
	generate_test_inputs.sh \
	package.m4 \
	test_macros.at

CLEANFILES = \
	$(check_AUTOTESTS) \
	*.exe \
	*.tmp \
	notify_stream.log \
	test_inputs_*.at

DISTCLEANFILES = \
	*.log \
	atconfig \
	Makefile \
	Makefile.in

