#!/bin/sh
# Script to run tests
#
# Version: 20201121

if test -f ${PWD}/libvmdk/.libs/libvmdk.1.dylib && test -f ./pyvmdk/.libs/pyvmdk.so;
then
	install_name_tool -change /usr/local/lib/libvmdk.1.dylib ${PWD}/libvmdk/.libs/libvmdk.1.dylib ./pyvmdk/.libs/pyvmdk.so;
fi

make check CHECK_WITH_STDERR=1;
RESULT=$?;

if test ${RESULT} -ne 0 && test -f tests/test-suite.log;
then
	cat tests/test-suite.log;
fi
exit ${RESULT};

