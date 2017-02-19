#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.64])

AC_INIT([IssueDB],
        [0.1.0],
        [allanjos@gmail.com],
        [issuedb],
        [https://www.olivum.com.br])

AM_INIT_AUTOMAKE([subdir-objects])

AC_CONFIG_SRCDIR([src/main_dialog.c])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CHECK_LIB(dl, dlopen)

AC_ARG_WITH([gtk2],
            [AS_HELP_STRING([--with-gtk2], [Build with the GTK+-3.0 interface])],
            [AC_DEFINE([with_gtk3], [], [Build with the GTK+-3.0 interface])])

AS_IF([test "x$with_gtk2" = "xyes"],
  [PKG_CHECK_MODULES([GTK], gtk+-2.0 >= 2.0.0)],
  [PKG_CHECK_MODULES([GTK], gtk+-3.0 >= 3.0.0)])

AC_CHECK_HEADERS([gtk/gtk.h])

AC_CONFIG_HEADERS([include/config.h])

AC_CONFIG_FILES([Makefile])

AM_INIT_AUTOMAKE

# AC_CONFIG_HEADERS([config.h])

AC_OUTPUT
