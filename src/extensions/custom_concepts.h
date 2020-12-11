#ifndef BANKMANAGEMENT_CUSTOM_CONCEPTS_H
#define BANKMANAGEMENT_CUSTOM_CONCEPTS_H

#include <iostream>

template <typename T>
concept Printable = requires(T t) {
    { std::cout << t };
};

template <typename T>
concept Inputable = requires(T t) {
    { std::cin >> t };
};

/// Taken from: https://github.com/koralkashri/cppsenioreas-iterators
template <template <typename ...> typename Container, typename T>
concept IteratorsFriendlyContainerType = requires (Container<T> container) {
    typename decltype(container)::pointer;
    typename decltype(container)::const_pointer;
    typename decltype(container)::reference;
    typename decltype(container)::const_reference;
    typename decltype(container)::iterator;
    typename decltype(container)::const_iterator;
    typename decltype(container)::reverse_iterator;
    typename decltype(container)::const_reverse_iterator;
    typename decltype(container)::size_type;
};

#endif //BANKMANAGEMENT_CUSTOM_CONCEPTS_H