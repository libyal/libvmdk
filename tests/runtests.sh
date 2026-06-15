#!/bin/sh
# Script to run tests
#
# Version: 20260609

if test -f ${PWD}/libvmdk/.libs/libvmdk.1.dylib && test -f ./pyvmdk/.libs/pyvmdk.so
then
	install_name_tool -change /usr/local/lib/libvmdk.1.dylib ${PWD}/libvmdk/.libs/libvmdk.1.dylib ./pyvmdk/.libs/pyvmdk.so
fi

make check-build > /dev/null

make check $@
RESULT=$?

if test ${RESULT} -ne 0
then
	find . -name \*.log -path \*.dir/\*/\*.log -print -exec cat {} \;
fi
exit ${RESULT}

