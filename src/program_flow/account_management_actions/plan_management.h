#ifndef BANKMANAGEMENT_PLAN_MANAGEMENT_H
#define BANKMANAGEMENT_PLAN_MANAGEMENT_H


#include "../../db/db_management.h"
#include "../../structures/output_logger_manager.h"
#include "../../external_tools/multimap/multimap.h"

class plan_management {
public:
    plan_management(db_management *db_ptr, const std::string &account_name, const std::string &plan_name, output_logger_manager *output);

    void choose_option();
    void print_details() const;

private:
    void increase_plan_cash() const;
    void decrease_plan_cash() const;
    void validate_account_free_cash(double required_cash_to_save) const;
    void validate_plan_cash_to_release(double desired_cash_to_release) const;
    void apply_action(const std::string &action) const;
    std::vector<keys_management<std::string>> get_available_options() const;

    multimap<std::string, void(plan_management::*)() const> available_actions;
    std::string exit_keyword;

    db_management *db_ptr;
    std::string account_name;
    std::string plan_name;
    output_logger_manager *output;
};

#endif //BANKMANAGEMENT_PLAN_MANAGEMENT_H