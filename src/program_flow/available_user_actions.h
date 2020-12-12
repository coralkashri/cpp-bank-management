#ifndef BANKMANAGEMENT_AVAILABLE_USER_ACTION_H
#define BANKMANAGEMENT_AVAILABLE_USER_ACTION_H

#include "../collections/bank.h"
#include "../extensions/custom_concepts.h"
#include "../structures/output_logger_manager.h"

class available_user_actions {
public:
    explicit available_user_actions(output_logger_manager *output) : output(output), bank_obj(output) {};

    void get_available_accounts();
    void create_account();
    void account_login();
    void account_logout();
    void remove_account();
    void get_available_plans();
    void get_available_plans_details();
    void plan_management();
    void create_plan();
    void remove_plan();
    bool is_logged_in();

private:
    bank bank_obj;
    account *current_account = nullptr;
    output_logger_manager *output;

    void account_login_validation();

    void account_logout_validation();
};

#endif //BANKMANAGEMENT_AVAILABLE_USER_ACTION_H