#ifndef DUNE_COMMON_TUPLES_NAMESPACE_HH
#define DUNE_COMMON_TUPLES_NAMESPACE_HH

#ifdef HAVE_TUPLE
#define DUNE_OPEN_TUPLE_NAMESPACE namespace std {
#define DUNE_CLOSE_TUPLE_NAMESPACE }
#elif defined HAVE_TR1_TUPLE // #ifdef HAVE_TUPLE
#define DUNE_OPEN_TUPLE_NAMESPACE namespace std { namespace tr1 {
#define DUNE_CLOSE_TUPLE_NAMESPACE } }
#else // #elif defined HAVE_TR1_TUPLE // #ifdef HAVE_TUPLE
#define DUNE_OPEN_TUPLE_NAMESPACE namespace Dune {
#define DUNE_CLOSE_TUPLE_NAMESPACE }
#endif // #else // #elif defined HAVE_TR1_TUPLE // #ifdef HAVE_TUPLE

#endif // #ifndef DUNE_COMMON_TUPLES_NAMESPACE_HH
