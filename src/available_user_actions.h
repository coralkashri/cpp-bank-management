#ifndef BANKMANAGEMENT_AVAILABLE_USER_ACTION_H
#define BANKMANAGEMENT_AVAILABLE_USER_ACTION_H

#include "bank.h"

namespace available_user_actions {
    std::vector<std::string> get_available_accounts(bank &bank_ref);
    bool is_account_exists(bank &bank_ref, std::string account_name);
    void create_account(bank &bank_ref);
    void get_available_plans(bank &bank_ref);
    void create_plan(bank &bank_ref);
    void plan_management(bank &bank_ref);
};

#endif //BANKMANAGEMENT_AVAILABLE_USER_ACTION_H