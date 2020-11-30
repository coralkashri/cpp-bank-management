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

class account_alreasy_exists : public std::exception {
    [[nodiscard]] const char* what() const noexcept override {
        return "This account already exists.";
    }
};

class logout_requirement_exception : public std::exception {
    [[nodiscard]] const char* what() const noexcept override {
        return "Please logout first.";
    }
};

class plan_not_found_exception : public std::exception {
    [[nodiscard]] const char* what() const noexcept override {
        return "Plan not found.";
    }
};

class plan_already_exists_exception : public std::exception {
    [[nodiscard]] const char* what() const noexcept override {
        return "This plan already exists.";
    }
};

#endif //BANKMANAGEMENT_CUSTOM_EXCEPTIONS_H