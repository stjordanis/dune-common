AC_DEFUN([MAKE_SHARED],[
  AC_REQUIRE([SHARED_PTR])
  AS_IF([test "$SHARED_PTR_NAMESPACE" = "boost"],[
    AC_CHECK_HEADER([boost/make_shared.hpp],
                  [AC_DEFINE([HAVE_BOOST_MAKE_SHARED_HPP], [1],
                        [Define to 1 if you have <boost/make_shared.hpp>.])])])
  AC_MSG_CHECKING([whether SHARED_PTR_NAMESPACE ($SHARED_PTR_NAMESPACE) provides make_shared])
  AC_LANG_PUSH([C++])
    AC_COMPILE_IFELSE(
      [AC_LANG_PROGRAM([[
#if defined(HAVE_MEMORY)
# include <memory>
#endif
#if defined(HAVE_TR1_MEMORY)
# include <tr1/memory>
#endif
#if defined(HAVE_BOOST_SHARED_PTR_HPP) && defined(HAVE_BOOST_MAKE_SHARED_HPP)
# include <boost/shared_ptr.hpp>
# include <boost/make_shared.hpp>
#endif
#include <string>
            ]],[[
$SHARED_PTR_NAMESPACE::make_shared<int>(3);
            ]])],
            [ AC_MSG_RESULT(yes)
              have_make_shared=yes
            ],[AC_MSG_RESULT(no)
              have_make_shared=no
            ])
  AS_IF([test "$have_make_shared" = "yes"],[
    AC_DEFINE([HAVE_MAKE_SHARED], [1],
                        [Define to 1 if SHARED_PTR_NAMESPACE::make_shared is usable.])])
  AC_LANG_POP
])
