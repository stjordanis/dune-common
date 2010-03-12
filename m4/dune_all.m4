## -*- autoconf -*-
# $Id$

# this meta-check calls everything needed for Dune to work and all
# possible components. Applications should use this so that
# Dune-updates enable new features automagically

# the entries are more or less copied from an "autoscan"-run in the
# dune-directory

AC_DEFUN([DUNE_CHECK_ALL],[
  AC_REQUIRE([DUNE_WEB])

  AC_ARG_ENABLE(enabledist,
    AS_HELP_STRING([--enable-dist],
                 [go into create-tarballs-mode [[default=no]]]))
  AM_CONDITIONAL(MAKEDIST, [test "x$enabledist" = "xyes"])
  AS_IF([test "x$enabledist" = "xyes"],[
    AM_CONDITIONAL(DUNEWEB, false)])

  # check the compilers (before using libtool !!!)
  AC_REQUIRE([PKG_PROG_PKG_CONFIG])
  AC_REQUIRE([AC_PROG_CC])
  AC_REQUIRE([AC_PROG_CPP])
  AC_REQUIRE([AC_PROG_CXX])
  AC_REQUIRE([AC_PROG_CXXCPP])
  AC_REQUIRE([AC_PROG_F77])
  # don't build shared libs per default, this is way better for debugging...
  AC_REQUIRE([AC_DISABLE_SHARED])
  # we need libtool
dnl do not use LT_INIT since we want to be compatible with libtool 1.5
  AC_REQUIRE([AC_PROG_LIBTOOL])
  AC_REQUIRE([LT_COMPAT])

  dnl check dependencies of this module
  dnl this test is autogenerated for each module
  AC_REQUIRE([DUNE_CHECK_MOD_DEPENDENCIES])

  # convenience-variables if every found package should be used
  AC_SUBST([ALL_PKG_LIBS], "$ALL_PKG_LIBS $DUNE_PKG_LIBS $LIBS")
  AC_SUBST([ALL_PKG_LDFLAGS], "$LDFLAGS $DUNE_PKG_LDFLAGS $ALL_PKG_LDFLAGS")
  AC_SUBST([ALL_PKG_CPPFLAGS], "$CPPFLAGS $DUNE_PKG_CPPFLAGS $ALL_PKG_CPPFLAGS")
  
  # convenience variables for all basic dune packages without extras
  AC_SUBST([DUNE_CPPFLAGS])
  AC_SUBST([DUNE_LDFLAGS])
  AC_SUBST([DUNE_LIBS])

  AC_SUBST([ACLOCAL_AMFLAGS], "$ACLOCAL_AMFLAGS")

  AC_SUBST([abs_srcdir])
  AC_SUBST([abs_top_srcdir])
  AC_SUBST([abs_builddir])
  AC_SUBST([abs_top_builddir])
  AC_SUBST(am_dir, $DUNE_COMMON_ROOT/am)
])

AC_DEFUN([DUNE_ADD_SUMMARY_ENTRY],[
  indentlen=24
  txt="$1"
  while test `echo "$txt" | tr -d '\n' | wc -c` -lt $indentlen; do txt="$txt."; done
  txt="$txt: $2"
  [DUNE_SUMMARY="$DUNE_SUMMARY echo '$txt';"]
])

AC_DEFUN([DUNE_ADD_SUMMARY_MOD_ENTRY],[
  indentlen=24
  txt=$1
  while test `echo $txt | tr -d '\n' | wc -c` -lt $indentlen; do txt=$txt.; done
  txt="$txt: $2"
  [DUNE_MODULES_SUMMARY="$DUNE_MODULES_SUMMARY echo '$txt';"]
])

AC_DEFUN([DUNE_SUMMARY_ALL],[
  # show search results

  echo
  echo "Found the following Dune-components: "
  echo
  echo "----------------------------------------"
  echo  
  [(eval $DUNE_MODULES_SUMMARY) | sort]
  [(eval $DUNE_SUMMARY) | sort]
  echo
  echo "----------------------------------------"
  echo
  echo "See ./configure --help and config.log for reasons why a component wasn't found"
  echo

])

AC_DEFUN([DUNE_CHECK_ALL_M],[
        [echo
        echo "    The build system of your module has called the deprecated method DUNE_CHECK_ALL_M."
        echo "    Please replace that call (in configure.ac) by a call to DUNE_CHECK_ALL.  If you"
        echo "    need the dune -> . symlink add DUNE_SYMLINK to your configure.ac."
        echo "    Press 'enter' to continue!"
        echo
        read -u 7]
  AC_REQUIRE([DUNE_SYMLINK])
  AC_REQUIRE([DUNE_CHECK_ALL])
  AC_REQUIRE([DUNE_AUTOBUILD_FLAGS])
])
