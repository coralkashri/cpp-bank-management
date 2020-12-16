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

template <typename T>
concept Numerical = std::is_arithmetic_v<T>;

#endif //BANKMANAGEMENT_CUSTOM_CONCEPTS_H