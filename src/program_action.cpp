#include "program_action.h"
#include <iostream>
#include <sstream>
#include "available_user_actions.h"
#include "custom_exceptions.h"

void program_action::run() {
    std::string action;
    while (!exit_flag) {
        std::cout << "Select action:\n" << get_available_actions() << std::endl;
        std::cin >> action;
        try {
            apply_action(action);
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
        }
    }
}

void program_action::apply_action(std::string action) {
    if (action == "get_available_accounts") {
        account_logout_validation();
        available_user_actions::get_available_accounts(my_bank);
    } else if (action == "create_account") {
        account_logout_validation();
        available_user_actions::create_account(my_bank);
    } else if (action == "get_plans") {
        account_login_validation();

    } else if (action == "logout") {
        account_login_validation();
        current_account = "";
    } else if (action == "account_login") {
        account_logout_validation();
        std::string desired_account;
        std::cout << "Enter account name: ";
        std::cin >> desired_account;
        if (available_user_actions::is_account_exists(my_bank, desired_account)) {
            current_account = desired_account;
            std::cout << "Successful login.";
        } else {
            throw account_not_found_exception();
        }
    }

    throw action_not_found_exception();
}

std::string program_action::get_available_actions() {
    std::stringstream str;
    // Bank actions
    if (current_account.empty()) {
        str << "- get_available_accounts" << "\n"
            << "- create_account" << "\n"
            << "- account_login" << "\n";
    }

    // Account action
    if (!current_account.empty()) {
        str << "- logout" << "\n"
            << "- get_plans" << "\n"
            << "- create_plan" << "\n";
    }

    return str.str();
}

void program_action::account_login_validation() {
    if (current_account.empty())
        throw account_not_selected_exception();
}

void program_action::account_logout_validation() {
    if (!current_account.empty())
        throw logout_requirement_exception();
}
