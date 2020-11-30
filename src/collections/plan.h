#ifndef BANKMANAGEMENT_PLAN_H
#define BANKMANAGEMENT_PLAN_H

#include <string>

class plan {
public:
    plan(std::string plan_name);

    std::string get_plan_name() const;

    bool operator==(const std::string &name);
    bool operator!=(const std::string &name);

private:
    std::string plan_name;
};

#endif //BANKMANAGEMENT_PLAN_H