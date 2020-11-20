#include "available_user_actions.h"
#include <iostream>

namespace available_user_actions {
    void available_user_actions::create_account(bank &bank_ref) {
        std::cout << "Enter account name: " << std::endl;
        std::string account_name;
        std::cin >> account_name;
        if (bank_ref.create_account(account_name)) {
            std::cout << "Account " << account_name << " successfully created.";
        } else {
            std::cout << "Account already exists.";
        }
    }
}