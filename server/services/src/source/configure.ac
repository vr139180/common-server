#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.63])
AC_INIT(EWOServer, 1.0, lujf1978@gmail.com)

AM_INIT_AUTOMAKE(EWOServer,1.0)

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_CPP

# Checks for libraries.
AC_PROG_RANLIB
AC_CHECK_LIB(pthread,pthread_atfork)

#AC_SUBST([AM_CXXFLAGS], [-fno-builtin-malloc -fno-builtin-calloc -fno-builtin-realloc -fno-builtin-free])

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T

# Checks for library functions.
AC_CHECK_FUNCS([socket])

AC_CONFIG_FILES([Makefile \
				ewwLogSystem/Makefile \
				ewwLib/Makefile \
				ewwLib/gsoap/Makefile \
                ewwGameLib/Makefile \
				ewwASPolicy/Makefile \
				ewwCenter/Makefile \
				ewwDBCache/Makefile \
				ewwGateway/Makefile \
				ewwGlobalSvr/Makefile \
				ewwGS/Makefile \
				ewwLoginSvr/Makefile \
				ewwService/Makefile
				])

AC_OUTPUT
