#include "available_user_actions.h"
#include <iostream>
#include "custom_exceptions.h"

bank bank_obj;
std::string current_account;

/*
 * Hidden Functions Declarations
 */
namespace available_user_actions {
    void account_login_validation();

    void account_logout_validation();

    bool is_account_exists(const std::string &account_name);
}

/*
 * Public Functions Definitions
 */
namespace available_user_actions {
    void create_account() {
        std::cout << "Enter account name: " << std::endl;
        std::string account_name;
        std::cin >> account_name;
        if (bank_obj.create_account(account_name)) {
            std::cout << "Account " << account_name << " successfully created.";
        } else {
            std::cout << "Account already exists.";
        }
    }

    void account_login() {
        account_logout_validation();
        std::string desired_account;
        std::cout << "Enter account name: ";
        std::cin >> desired_account;
        if (available_user_actions::is_account_exists(desired_account)) {
            current_account = desired_account;
            std::cout << "Successful login.";
        } else {
            throw account_not_found_exception();
        }
    }

    void get_available_accounts() {
        account_logout_validation();
        // TODO
    }

    void get_available_plans() {
        account_login_validation();
        // TODO
    }

    void create_plan() {
        account_login_validation();
        // TODO
    }

    void plan_management() {
        account_login_validation();
        // TODO
    }

    void account_logout() {
        account_login_validation();
        // TODO
    }

    void remove_account() {
        account_logout_validation();
        // TODO
    }

    void remove_plan() {
        account_login_validation();
        // TODO
    }
}

/*
 * Hidden Functions Definitions
 */
namespace available_user_actions {
    void account_login_validation() {
        if (current_account.empty())
            throw account_not_selected_exception();
    }

    void account_logout_validation() {
        if (!current_account.empty())
            throw logout_requirement_exception();
    }

    bool is_account_exists(const std::string &account_name) {
        // TODO
        return false;
    }
}