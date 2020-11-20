#ifndef BANKMANAGEMENT_PROGRAM_ACTION_H
#define BANKMANAGEMENT_PROGRAM_ACTION_H

#include <map>
#include <string>
#include <functional>
#include "bank.h"

class program_action {
public:
    program_action() = default;

    void run();

private:
    bank my_bank;
    std::string current_account;
    bool exit_flag = false;
    std::map<std::string, std::function<void()>> available_actions;

    std::string get_available_actions();
    void apply_action(std::string action);

    void account_login_validation();
    void account_logout_validation();

};

#endif //BANKMANAGEMENT_PROGRAM_ACTION_H