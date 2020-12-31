#include "plan_management.h"
#include "../../extensions/std_extensions.h"
#include "../../extensions/my_shell.h"
#include "../../extensions/custom_exceptions.h"
#include "../../extensions/custom_validations.h"

plan_management::plan_management(db_management *db_ptr, const std::string &account_name, const std::string &plan_name,
                                 output_logger_manager *output)
                                 : account_management_actions_interface<plan_management>(db_ptr, account_name, output), plan_name(plan_name) {
    available_actions = {
            {{{"add_cash",      "ac", ""}, "Move cash from free cash in this account to this plan."}, &plan_management::increase_plan_cash},
            {{{"decrease_cash", "dc", ""}, "Move cash from this plan to this account's free cash."},  &plan_management::decrease_plan_cash}
    };
}

void plan_management::increase_plan_cash() const {
    double cash;
    my_shell::input("cash to add", cash);
    if (!custom_validations::is_positive(cash))
        throw expected_positive_number_exception();
    validate_account_free_cash(cash);
    db_ptr->modify_plan_balance(account_name, plan_name, cash);
    db_ptr->modify_account_free_cash(account_name, -cash);
}

void plan_management::decrease_plan_cash() const {
    double cash;
    my_shell::input("cash to decrease", cash);
    if (!custom_validations::is_positive(cash))
        throw expected_positive_number_exception();
    validate_plan_cash_to_release(cash);
    db_ptr->modify_plan_balance(account_name, plan_name, -cash);
    db_ptr->modify_account_free_cash(account_name, cash);
}

void plan_management::print_details() const {
    double cash = db_ptr->get_plan_balance(account_name, plan_name);
    output->printer() << "====================================\n";
    output->printer() << "Account: " << account_name << "\n";
    output->printer() << "Plan: " << plan_name << "\n";
    output->printer() << "Cash: " << cash << "\n";
    output->printer() << "====================================\n";
}

void plan_management::validate_account_free_cash(double required_cash_to_save) const {
    if (db_ptr->get_account_free_cash(account_name) < required_cash_to_save)
        throw not_enough_free_cash_in_account_exception();
}

void plan_management::validate_plan_cash_to_release(double desired_cash_to_release) const {
    if (db_ptr->get_plan_balance(account_name, plan_name) < desired_cash_to_release)
        throw not_enough_cash_in_plan_exception();
}
