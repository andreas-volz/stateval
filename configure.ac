dnl Process this file with autoconf to produce a configure script.
dnl Created by Anjuta application wizard.

AC_INIT(stateval, 0.1)

AM_INIT_AUTOMAKE(AC_PACKAGE_NAME, AC_PACKAGE_VERSION)
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
AM_MAINTAINER_MODE

AC_ISC_POSIX
AC_PROG_CXX
AM_PROG_CC_STDC
AC_HEADER_STDC






AM_PROG_LIBTOOL

#dnl Set PACKAGE SOURCE DIR in config.h.
packagesrcdir=`cd $srcdir && pwd`

#dnl Set PACKAGE PREFIX
if test "x${prefix}" = "xNONE"; then
  packageprefix=${ac_default_prefix}
else
  packageprefix=${prefix}
fi

#dnl Set PACKAGE DIRS in config.h
packagedatadir=share/${PACKAGE}
packagedocdir=doc/${PACKAGE}
packagepluginsdir=${packagelibdir}/plugins

#dnl Subst PACKAGE_DATA_DIR.
NO_PREFIX_PACKAGE_DATA_DIR="${packagedatadir}"
AC_SUBST(NO_PREFIX_PACKAGE_DATA_DIR)
PACKAGE_DATA_DIR="${packageprefix}/${packagedatadir}"
AC_SUBST(PACKAGE_DATA_DIR)

dnl Subst PACKAGE_DOC_DIR.
NO_PREFIX_PACKAGE_DOC_DIR="${packagedocdir}"
AC_SUBST(NO_PREFIX_PACKAGE_DOC_DIR)
PACKAGE_DOC_DIR="${packageprefix}/${packagedocdir}"
AC_SUBST(PACKAGE_DOC_DIR)

dnl Subst PACKAGE_PLUGINS_DIR.
NO_PREFIX_PACKAGE_PLUGINS_DIR="${packagepluginsdir}"
AC_SUBST(NO_PREFIX_PACKAGE_PLUGINS_DIR)
PACKAGE_PLUGINS_DIR="${packageprefix}/${packagepluginsdir}"
AC_SUBST(PACKAGE_PLUGINS_DIR)

AC_DEFINE_UNQUOTED(PACKAGE_DATA_DIR, "${packageprefix}/${packagedatadir}","")
AC_DEFINE_UNQUOTED(SYSTEM_DATA_DIR, "${packageprefix}/${systemdatadir}","")
AC_DEFINE_UNQUOTED(PACKAGE_DOC_DIR, "${packageprefix}/${packagedocdir}","")
AC_DEFINE_UNQUOTED(PACKAGE_PLUGINS_DIR, "${packageprefix}/${packagepluginsdir}","")
AC_DEFINE_UNQUOTED(SYSTEM_PLUGINS_DIR, "${packageprefix}/${systempluginsdir}","")
AC_DEFINE_UNQUOTED(PACKAGE_SOURCE_DIR, "${packagesrcdir}","")

## TODO: ecorexx only needed for edje_console example...
PKG_CHECK_MODULES([EFL], elementaryxx ecorexx,
     AM_CONDITIONAL(HAVE_EFL, [test 1 = 1]),
     AM_CONDITIONAL(HAVE_EFL, [test 0 = 1]),
)

PKG_CHECK_MODULES([XML], libxml++-2.6,
     AM_CONDITIONAL(HAVE_XML, [test 1 = 1]),
     AM_CONDITIONAL(HAVE_XML, [test 0 = 1])
)

dnl Check for option to enable logging
AC_MSG_CHECKING(whether to enable logging)
AC_ARG_ENABLE(logging,
        [  --enable-logging=[no/yes]        enables logging build (default=yes)],,
                 enable_logging=yes)              

if [ ! test "$enable_logging" != "yes"]; then
  AC_MSG_RESULT(yes)
  AC_DEFINE(HAVE_LOGGING, 1, [Define to enable logging build])
  PKG_CHECK_MODULES([LOG4CXX], liblog4cxx,
        AC_DEFINE(HAVE_LOG4CXX, 1, [Define to enable logging build]),
        [test 1 = 1]
  )
fi


dnl Check for option to enable test applications
AC_MSG_CHECKING(whether to enable test applications)
AC_ARG_ENABLE(tests,
        [  --enable-tests=[no/yes]        enables test applications build (default=yes)],,
                 enable_tests=yes)

if [ ! test "$enable_tests" != "yes"]; then
  AC_MSG_RESULT(yes)
  AC_DEFINE(HAVE_CPPUNIT, 1, [Define to enable test application build])
  PKG_CHECK_MODULES([CPPUNIT], cppunit,
	 AM_CONDITIONAL(HAVE_CPPUNIT, [test 1 = 1]),
	 AM_CONDITIONAL(HAVE_CPPUNIT, [test 0 = 1])
  )
fi

ACX_PTHREAD([],[])

PKG_CHECK_MODULES(SIGC, sigc++-2.0)

PKG_CHECK_MODULES(PLUXX, pluxx)

AC_OUTPUT([
Makefile
data/Makefile
include/Makefile
include/stateval/Makefile
include/stateval/private/Makefile
src/Makefile
src/plugins/Makefile
src/plugins/loaders/Makefile
src/plugins/loaders/smxml/Makefile
src/plugins/loaders/smtest/Makefile
src/plugins/views/Makefile
src/plugins/views/edje/Makefile
src/plugins/views/text/Makefile
stateval.pc
stateval-uninstalled.pc
examples/Makefile
examples/edje_console/Makefile
examples/text_views/Makefile
test/Makefile
test/module/Makefile
])
