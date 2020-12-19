#ifndef BANKMANAGEMENT_MY_SHELL_H
#define BANKMANAGEMENT_MY_SHELL_H

#include "custom_concepts.h"

namespace my_shell {
    template <Inputable T>
    void method_input(T &input) {
        std::cout << "> ";
        std::cin >> input;
    }

    template <Inputable T>
    inline void input(const std::string &input_request, T &variable) {
        std::cout << "Please enter " << input_request << ": ";
        std::cin >> variable;
    }

    template<>
    inline void input(const std::string &input_request, std::string &variable) {
        std::cout << "Please enter " << input_request << ": ";
        std::getline(std::cin >> std::ws, variable);
    }
};

#endif //BANKMANAGEMENT_MY_SHELL_H