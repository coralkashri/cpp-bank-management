#ifndef BANKMANAGEMENT_STD_EXTENSIONS_H
#define BANKMANAGEMENT_STD_EXTENSIONS_H

#include <iostream>
#include <vector>
#include "custom_concepts.h"

namespace std {
    template <Printable T>
    ostream& operator<<(ostream &out, vector<T> vec) {
        size_t number = 1;
        for (auto &elem : vec) {
            out << number++ << ".\t" << elem << "\n";
        }
        return out;
    }

    template <Inputable T>
    void input(std::string input_request, T &variable) {
        std::cout << "Please enter " << input_request << ": ";
        std::cin >> variable;
    }
}

#endif //BANKMANAGEMENT_STD_EXTENSIONS_H