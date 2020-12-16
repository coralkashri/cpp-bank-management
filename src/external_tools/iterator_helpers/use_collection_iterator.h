/**
 * Origin:
 * https://github.com/koralkashri/cppsenioreas-iterators
 */

#ifndef BANKMANAGEMENT_USE_COLLECTION_ITERATOR_H
#define BANKMANAGEMENT_USE_COLLECTION_ITERATOR_H

#include <type_traits>
#include <iterator>

template <template <typename ...> typename Container, typename K, typename V>
struct use_collection_iterator {
    typedef typename Container<K, V>::pointer pointer;
    typedef typename Container<K, V>::const_pointer const_pointer;
    typedef typename Container<K, V>::reference reference;
    typedef typename Container<K, V>::const_reference const_reference;
    typedef typename Container<K, V>::iterator iterator;
    typedef typename Container<K, V>::const_iterator const_iterator;
    typedef typename Container<K, V>::reverse_iterator reverse_iterator;
    typedef typename Container<K, V>::const_reverse_iterator const_reverse_iterator;
    typedef typename Container<K, V>::size_type size_type;
};

template <template <typename ...> typename Container, typename K, typename V>
struct use_collection_iterator_without_reverse {
    typedef typename Container<K, V>::pointer pointer;
    typedef typename Container<K, V>::const_pointer const_pointer;
    typedef typename Container<K, V>::reference reference;
    typedef typename Container<K, V>::const_reference const_reference;
    typedef typename Container<K, V>::iterator iterator;
    typedef typename Container<K, V>::const_iterator const_iterator;
    typedef typename Container<K, V>::size_type size_type;
};

#endif //BANKMANAGEMENT_USE_COLLECTION_ITERATOR_H
