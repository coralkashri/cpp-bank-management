#ifndef BANKMANAGEMENT_CUSTOM_EXCEPTIONS_H
#define BANKMANAGEMENT_CUSTOM_EXCEPTIONS_H

#include <iostream>

class action_not_found_exception : public std::exception {
    [[nodiscard]] const char* what() const noexcept override {
        return "Action not found";
    }
};

class account_not_selected_exception : public std::exception {
    [[nodiscard]] const char* what() const noexcept override {
        return "Please select account first.";
    }
};

class account_not_found_exception : public std::exception {
    [[nodiscard]] const char* what() const noexcept override {
        return "Account not found.";
    }
};

class logout_requirement_exception : public std::exception {
    [[nodiscard]] const char* what() const noexcept override {
        return "Please logout first.";
    }
};

#endif //BANKMANAGEMENT_CUSTOM_EXCEPTIONS_H