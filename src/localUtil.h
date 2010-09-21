#ifndef LOCAL_UTIL_H
#define LOCAL_UTIL_H

/* STL */
#include <algorithm>
#include <functional>

// some generic template functions for delete algorithms
template <typename T>
void delete_one (T *t)
{
  delete t;
  t = NULL;
}

template <typename T>
void delete_array (T *t)
{
  delete [] t;
  t = NULL;
}

template <typename T>
struct delete_unary : public std::unary_function <T, void>
{
  void operator () (T &t)
  {
    delete t;
  }
};

template <typename T, typename S>
void delete_stl_container (T &t)
{
  for_each (t.begin (), t.end (), delete_unary <S> ());
  t.clear ();
}

#endif // LOCAL_UTIL_H
