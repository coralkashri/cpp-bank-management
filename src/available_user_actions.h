#ifndef BANKMANAGEMENT_AVAILABLE_USER_ACTION_H
#define BANKMANAGEMENT_AVAILABLE_USER_ACTION_H

#include "bank.h"

namespace available_user_actions {
    void get_available_accounts();
    void create_account();
    void account_login();
    void account_logout();
    void remove_account();
    void get_available_plans();
    void plan_management();
    void create_plan();
    void remove_plan();
};

#endif //BANKMANAGEMENT_AVAILABLE_USER_ACTION_H