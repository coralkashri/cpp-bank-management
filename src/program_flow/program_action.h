#ifndef BANKMANAGEMENT_PROGRAM_ACTION_H
#define BANKMANAGEMENT_PROGRAM_ACTION_H

#include <map>
#include <string>
#include <functional>
#include "../collections/bank.h"
#include "available_user_actions.h"

class program_action {
public:
    program_action(std::string &&db_path);

    void run();

private:
    bool is_running_flag = false;
    available_user_actions user_actions;
    std::map<std::string, void(available_user_actions::*)()> available_actions;

    std::string get_available_actions();
    void apply_action(const std::string &action);
    void exit();
    [[nodiscard]] bool is_running() const;
};

#endif //BANKMANAGEMENT_PROGRAM_ACTION_H