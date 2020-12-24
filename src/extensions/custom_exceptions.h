#ifndef BANKMANAGEMENT_CUSTOM_EXCEPTIONS_H
#define BANKMANAGEMENT_CUSTOM_EXCEPTIONS_H

#include <iostream>
#include "../program_flow/structures/transaction.h"

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

class table_field_type_matching_exception : public std::exception {
    [[nodiscard]] const char* what() const noexcept override {
        return "Table field type doesn't mach expected type.";
    }
};

class not_enough_free_cash_in_account_exception : public std::exception {
    [[nodiscard]] const char* what() const noexcept override {
        return "You don't have enough money in this account to apply the last action.";
    }
};

class not_enough_cash_in_plan_exception : public std::exception {
    [[nodiscard]] const char* what() const noexcept override {
        return "You don't have enough money in this plan to apply the last action.";
    }
};

class expected_positive_number_exception : public std::exception {
    [[nodiscard]] const char* what() const noexcept override {
        return "Illegal action: expected positive number.";
    }
};

class expected_negative_number_exception : public std::exception {
    [[nodiscard]] const char* what() const noexcept override {
        return "Illegal action: expected negative number.";
    }
};

class transaction_not_found_exception : public std::exception {
public:
    transaction_not_found_exception(const transaction_id& id) : t_id(id) {
        msg = t_id.is_income ? "Income" : "Outcome";
        msg += " transaction: " + t_id.transaction_name + " not found.";
    }

    [[nodiscard]] const char* what() const noexcept override {
        return msg.c_str();
    }

private:
    transaction_id t_id;
    std::string msg;
};

class transaction_already_exists_exception : public std::exception {
public:
    transaction_already_exists_exception(const transaction_id& id) : t_id(id) {
        msg = t_id.is_income ? "Income" : "Outcome";
        msg += " transaction: " + t_id.transaction_name + " already exists.";
    }

    [[nodiscard]] const char* what() const noexcept override {
        return msg.c_str();
    }

private:
    transaction_id t_id;
    std::string msg;
};

#endif //BANKMANAGEMENT_CUSTOM_EXCEPTIONS_H