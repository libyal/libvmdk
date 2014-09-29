dnl Function to detect if libvmdk dependencies are available
AC_DEFUN([AX_LIBVMDK_CHECK_LOCAL],
 [dnl Check for internationalization functions in libvmdk/libvmdk_i18n.c 
 AC_CHECK_FUNCS([bindtextdomain])
 ])

