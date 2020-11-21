#include "available_user_actions.h"
#include <iostream>
#include <algorithm>
#include "custom_exceptions.h"
#include "custom_concepts.h"

bank bank_obj;
account *current_account;

/*
 * Hidden Functions Declarations
 */
namespace available_user_actions {
    void account_login_validation();

    void account_logout_validation();

    bool is_account_exists(const std::string &account_name);

    template <Printable T>
    std::ostream& operator<<(std::ostream &cout, std::vector<T> vec);

    template <Inputable T>
    void input(std::string input_request, T &variable);
}

/*
 * Public Functions Definitions
 */
namespace available_user_actions {
    void create_account() {
        std::cout << "Enter account name: " << std::endl;
        std::string account_name;
        input("account name", account_name);
        if (bank_obj.create_account(account_name)) {
            std::cout << "Account " << account_name << " successfully created.";
        } else {
            std::cout << "Account already exists.";
        }
    }

    void account_login() {
        account_logout_validation();
        std::string desired_account;
        input("account name", desired_account);
        if (available_user_actions::is_account_exists(desired_account)) {
            current_account = &bank_obj.get_account(desired_account);
            std::cout << "Successful login.";
        } else {
            throw account_not_found_exception();
        }
    }

    void get_available_accounts() {
        account_logout_validation();
        std::cout << bank_obj.get_accounts_names();
    }

    void get_available_plans() {
        account_login_validation();
        std::cout << current_account->get_plan_names();
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
        current_account = nullptr;
        std::cout << "Successfully logout.";
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
        if (!current_account)
            throw account_not_selected_exception();
    }

    void account_logout_validation() {
        if (current_account)
            throw logout_requirement_exception();
    }

    bool is_account_exists(const std::string &account_name) {
        // TODO
        return false;
    }

    template <Printable T>
    std::ostream& operator<<(std::ostream &cout, std::vector<T> vec) {
        size_t number = 1;
        for (auto &elem : vec) {
            cout << number << ".\t" << elem << "\n";
        }
        return cout;
    }

    template <Inputable T>
    void input(std::string input_request, T &variable) {
        std::cout << "Please enter " << input_request << ": ";
        std::cin >> variable;
    }
}