#include "program_action.h"
#include <iostream>
#include "../extensions/custom_exceptions.h"
#include "../extensions/std_extensions.h"
#include "../extensions/my_shell.h"

using namespace std::string_literals;

program_action::program_action() : user_actions(&output) {
    available_bank_actions = {
            {{{"get_available_accounts", "gaa",  ""}, "Print registered account names."},               &available_user_actions::get_available_accounts},
            {{{"create_account",         "ca",   ""}, "Create a new account."},                         &available_user_actions::create_account},
            {{{"remove_account",         "ra",   ""}, "Delete account and it's cash from the system."}, &available_user_actions::remove_account},
            {{{"account_login",          "alin", ""}, "Manage selected account."},                      &available_user_actions::account_login},
    };
    available_account_actions = {
            {{{"logout",                  "lout", ""}, "Back to accounts management."},                      &available_user_actions::account_logout},
            {{{"add_free_cash",           "afc",  ""}, "Increase this account free cash."},                  &available_user_actions::increase_free_cash},
            {{{"decrease_free_cash",      "dfc",  ""}, "Decrease this account free cash."},                  &available_user_actions::decrease_free_cash},
            {{{"get_plans",               "gp",   ""}, "Get all plan names in this account."},               &available_user_actions::get_available_plans},
            {{{"get_plans_details",       "gpd",  ""}, "Get all plans details in this account."},            &available_user_actions::get_available_plans_details},
            {{{"create_plan",             "cp",   ""}, "Create new plan and associate it to this account."}, &available_user_actions::create_plan},
            {{{"remove_plan",             "rp",   ""}, "Remove plan from this account."},                    &available_user_actions::remove_plan},
            {{{"plan_management",         "pm",   ""}, "Manage selected plan in this account."},             &available_user_actions::plan_management},
            {{{"transactions_management", "tm",   ""}, "Update a monthly income to this account."},          &available_user_actions::account_transactions_management}
    };
    is_running_flag = true;
    exit_keyword = "exit";
}

void program_action::run() {
    std::string action;
    while (is_running()) {
        if (is_account_management_state()) {
            user_actions.print_account_details();
        }
        std::cout << "Select action:\n" << get_available_actions() << std::endl;
        my_shell::method_input(action);
        if (!is_account_management_state() && action == exit_keyword) this->exit();
        else try {
            apply_action(action);
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
        }
    }
}

std::vector<keys_management<std::string>> program_action::get_available_actions() {
    std::vector<keys_management<std::string>> options_vec;

    if (!is_account_management_state()) {
        // Bank actions
        options_vec.emplace_back(keys_management<std::string>{exit_keyword});
        available_actions_set = &available_bank_actions;
    } else {
        // Account action
        available_actions_set = &available_account_actions;
    }
    auto options = available_actions_set->get_key_relations();
    options_vec.insert(options_vec.begin(), options.begin(), options.end());
    return options_vec;
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

bool program_action::is_account_management_state() {
    return user_actions.is_logged_in();
}