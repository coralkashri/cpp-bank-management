#ifndef BANKMANAGEMENT_PLAN_MANAGEMENT_H
#define BANKMANAGEMENT_PLAN_MANAGEMENT_H


#include "../../db/db_management.h"
#include "../../structures/output_logger_manager.h"
#include "../../external_tools/multimap/multimap.h"
#include "account_management_actions_interface.h"

class plan_management : public account_management_actions_interface<plan_management> {
public:
    friend account_management_actions_interface<plan_management>;

    plan_management(db_management *db_ptr, const std::string &account_name, const std::string &plan_name, output_logger_manager *output);

    void print_details() const;

private:
    multimap<std::string, void(plan_management::*)() const> available_actions;
    void increase_plan_cash() const;
    void decrease_plan_cash() const;
    void validate_account_free_cash(double required_cash_to_save) const;
    void validate_plan_cash_to_release(double desired_cash_to_release) const;

    std::string plan_name;
};

#endif //BANKMANAGEMENT_PLAN_MANAGEMENT_H