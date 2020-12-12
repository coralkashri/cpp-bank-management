#ifndef BANKMANAGEMENT_PLAN_MANAGEMENT_H
#define BANKMANAGEMENT_PLAN_MANAGEMENT_H


#include "../db/db_management.h"
#include "../structures/output_logger_manager.h"

class plan_management {
public:
    plan_management(db_management *db_ptr, const std::string &account_name, const std::string &plan_name, output_logger_manager *output);

    void choose_option();
    void print_details() const;

private:
    void increase_plan_cash() const;
    void decrease_plan_cash() const;
    void apply_action(const std::string &action) const;

    std::map<std::string, void(plan_management::*)() const> available_actions;

    db_management *db_ptr;
    std::string account_name;
    std::string plan_name;
    output_logger_manager *output;
};

#endif //BANKMANAGEMENT_PLAN_MANAGEMENT_H