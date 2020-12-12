#include <fstream>
#include "account.h"
#include "../extensions/custom_exceptions.h"

account::account(db_management *db_ptr, const std::string &account_name, output_logger_manager *output)
: db_ptr(db_ptr), account_name(account_name), output(output) {
    try {
        // Create account in db
        db_ptr->create_account(account_name);
    } catch (account_alreasy_exists &e) {
        double available_cash = db_ptr->get_account_free_cash(account_name);
        output->printer() << "Account already exists, free cash: " << available_cash << ".\n";
    }
}

void account::print_account_details() {
    double cash = db_ptr->get_account_free_cash(account_name);
    output->printer() << "====================================\n";
    output->printer() << "Account: " << account_name << "\n";
    output->printer() << "Free Cash: " << cash << "\n";
    output->printer() << "====================================\n";
}

bool account::delete_account() {
    // Remove account from db
    db_ptr->delete_account(account_name);
    return true;
}

std::string account::get_account_name() const {
    return account_name;
}

bool account::operator==(const std::string &name) {
    return name == account_name;
}

std::vector<std::string> account::get_plan_names() const {
    std::vector<std::string> plan_names;
    for (auto &plan : plans) {
        plan_names.emplace_back(plan.get_plan_name());
    }
    return plan_names;
}

bool account::operator!=(const std::string &name) {
    return !(*this == name);
}

void account::create_plan(const std::string &plan_name) {
    plans.emplace_back(db_ptr, account_name, plan_name, output);
}

void account::print_plan_details(const std::string &plan_name) {
    auto plan_it = find_plan(plan_name);
    plan_it->print_details();
}

void account::plan_management(const std::string &plan_name) {
    auto plan_it = find_plan(plan_name);
    plan_it->plan_actions();
}

void account::remove_plan(const std::string &plan_name) {
    db_ptr->delete_plan(account_name, plan_name);
    auto plan_it = find_plan(plan_name);
    double cash = plan_it->get_plan_cash();
    plans.erase(plan_it);
    // todo available_cash += cash;
}

std::vector<plan>::iterator account::find_plan(const std::string &name) {
    if (auto it = std::find(plans.begin(), plans.end(), name); it != plans.end()) {
        return it;
    }
    throw plan_not_found_exception();
}