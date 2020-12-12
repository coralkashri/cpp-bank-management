#ifndef BANKMANAGEMENT_PROGRAM_ACTION_H
#define BANKMANAGEMENT_PROGRAM_ACTION_H

#include <map>
#include <string>
#include <functional>
#include "../collections/bank.h"
#include "available_user_actions.h"
#include "../structures/output_logger_manager.h"

class program_action {
public:
    program_action();

    void run();

private:
    bool is_running_flag = false;
    output_logger_manager output;
    available_user_actions user_actions;
    std::map<std::string, void(available_user_actions::*)()> available_bank_actions;
    std::map<std::string, void(available_user_actions::*)()> available_account_actions;
    std::map<std::string, void(available_user_actions::*)()> *available_actions_set;
    std::string exit_keyword;

    std::vector<std::string> get_available_actions();
    void apply_action(const std::string &action);
    void exit();
    [[nodiscard]] bool is_running() const;
    [[nodiscard]] bool is_account_management_state();
};

#endif //BANKMANAGEMENT_PROGRAM_ACTION_H