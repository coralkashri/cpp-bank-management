#include "account.h"
#include "../extensions/custom_exceptions.h"

account::account(const std::string &db_path, const std::string &account_name) : db_path(db_path), account_name(account_name) {

}

bool account::operator==(const std::string &name) {
    return name == account_name;
}

bool account::operator!=(const std::string &name) {
    return !(*this == name);
}

void account::create_plan(const std::string &plan_name) {
    try {
        find_plan(plan_name);
        throw plan_already_exists_exception();
    } catch (plan_not_found_exception &e) {
        // todo
    }
}

std::vector<plan>::iterator account::find_plan(const std::string &name) {
    if (auto it = std::find(plans.begin(), plans.end(), name); it != plans.end()) {
        return it;
    }
    throw plan_not_found_exception();
}

std::string account::get_account_name() const {
    return account_name;
}

std::vector<std::string> account::get_plan_names() const {
    return std::vector<std::string>();
}

void account::print_plan_details(std::string plan_name) {

}

void account::plan_management(const std::string &plan_name) {

}

void account::remove_plan(std::string plan_name) {

}
