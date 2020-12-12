#include "program_action.h"
#include <iostream>
#include <sstream>
#include "../extensions/custom_exceptions.h"
#include "../extensions/std_extensions.h"

program_action::program_action() : user_actions(&output) {
    available_bank_actions = {
            {"get_available_accounts", &available_user_actions::get_available_accounts},
            {"create_account",         &available_user_actions::create_account},
            {"remove_account",         &available_user_actions::remove_account},
            {"account_login",          &available_user_actions::account_login},
    };
    available_account_actions = {
            {"logout",            &available_user_actions::account_logout},
            {"get_plans",         &available_user_actions::get_available_plans},
            {"get_plans_details", &available_user_actions::get_available_plans_details},
            {"create_plan",       &available_user_actions::create_plan},
            {"remove_plan",       &available_user_actions::remove_plan},
            {"plan_management",   &available_user_actions::plan_management}
    };
    is_running_flag = true;
    exit_keyword = "exit";
}

void program_action::run() {
    std::string action;
    while (is_running()) {
        std::cout << "Select action:\n" << get_available_actions() << std::endl;
        std::cin >> action;
        if (action == exit_keyword) this->exit();
        else try {
            apply_action(action);
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
        }
    }
}

std::string program_action::get_available_actions() {
    std::stringstream str;
    std::vector<std::string> options_vec;

    if (!user_actions.is_logged_in()) {
        // Bank actions
        options_vec.emplace_back(exit_keyword);
        available_actions_set = &available_bank_actions;
    } else {
        // Account action
        available_actions_set = &available_account_actions;
    }
    auto options = *available_actions_set | std::views::keys | std::views::common;
    options_vec.insert(options_vec.begin(), options.begin(), options.end());
    str << options_vec;
    return str.str();
}

void program_action::apply_action(const std::string &action) {
    try {
        (user_actions.*(available_actions_set->at(action)))();
    } catch (std::out_of_range &e) {
        throw action_not_found_exception();
    }
}

void program_action::exit() {
    is_running_flag = false;
}

bool program_action::is_running() const {
    return is_running_flag;
}
