#include "program_action.h"
#include <iostream>
#include <sstream>
#include "../extensions/custom_exceptions.h"

program_action::program_action(std::string &&db_path) : user_actions(std::move(db_path)) {
    available_actions = {
            {"get_available_accounts", &available_user_actions::get_available_accounts},
            {"create_account",         &available_user_actions::create_account},
            {"remove_account",         &available_user_actions::remove_account},
            {"account_login",          &available_user_actions::account_login},
            {"logout",                 &available_user_actions::account_logout},
            {"get_plans",              &available_user_actions::get_available_plans},
            {"create_plan",            &available_user_actions::create_plan},
            {"remove_plan",            &available_user_actions::remove_plan},
            {"plan_management",        &available_user_actions::plan_management}
    };
    is_running_flag = true;
}

void program_action::run() {
    std::string action;
    while (is_running()) {
        std::cout << "Select action:\n" << get_available_actions() << std::endl;
        std::cin >> action;
        if (action == "exit") this->exit();
        else try {
            apply_action(action);
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
        }
    }
}

std::string program_action::get_available_actions() {
    std::stringstream str;

    if (!user_actions.is_logged_in()) {
        // Bank actions
        str << "- get_available_accounts" << "\n"
            << "- create_account" << "\n"
            << "- remove_account" << "\n"
            << "- account_login" << "\n"
            << "- exit" << "\n";
    } else {
        // Account action
        str << "- logout" << "\n"
            << "- get_plans" << "\n"
            << "- create_plan" << "\n"
            << "- remove_plan" << "\n"
            << "- plan_management" << "\n";
    }

    return str.str();
}

void program_action::apply_action(const std::string &action) {
    if (auto it = available_actions.find(action); it != available_actions.end()) {
        (user_actions.*(it->second))();
    } else {
        throw action_not_found_exception();
    }
}

void program_action::exit() {
    is_running_flag = false;
}

bool program_action::is_running() const {
    return is_running_flag;
}
