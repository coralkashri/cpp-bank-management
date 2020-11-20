#ifndef BANKMANAGEMENT_ACCOUNT_H
#define BANKMANAGEMENT_ACCOUNT_H

#include <string>
#include <vector>
#include "plan.h"

class account {
public:
    account(std::string account_name);

    std::vector<std::string> get_plan_names() const;
    plan get_plan_details(std::string plan_name);
    void create_plan(std::string plan_name);
    void remove_plan(std::string plan_name);


private:
    std::vector<plan> plans;
    std::string account_name;
};

#endif //BANKMANAGEMENT_ACCOUNT_H