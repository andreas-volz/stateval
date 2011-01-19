#ifndef MEMORY_UTIL_H
#define MEMORY_UTIL_H

/* STL */
#include <map>

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
void delete_stl_container (T &cnt)
{
  for (typename T::iterator it = cnt.begin(); it != cnt.end(); ++it)
  {
    delete *it;
  }
  cnt.clear();
}

/*!
 * Delete the STL std::map container elements and clear it afterwards.
 * But note that only the value elements (->second) are deleted!!!!
 */
template <typename Key, typename T>
void delete_stl_container (std::map<Key, T> &cnt)
{
  for (typename std::map<Key, T>::iterator it = cnt.begin(); it != cnt.end(); ++it)
  {
    delete it->second;
  }
  cnt.clear ();
}

template <typename Key, typename T>
void delete_stl_container (std::multimap<Key, T> &cnt)
{
  for (typename std::multimap<Key, T>::iterator it = cnt.begin(); it != cnt.end(); ++it)
  {
    delete it->second;
  }
  cnt.clear ();
}

#endif // MEMORY_UTIL_H
