#include "bank.h"
#include "../extensions/custom_exceptions.h"

bank::bank() {
    restore_data_from_db();
}

std::vector<std::string> bank::get_account_names() const {
    std::vector<std::string> account_names;
    for (auto &account : accounts) {
        account_names.emplace_back(account.get_account_name());
    }
    return account_names;
}

account &bank::get_account(const std::string &account_name) {
    return *find_account(account_name);
}

void bank::create_account(const std::string &account_name) {
    try {
        find_account(account_name);
        throw account_alreasy_exists();
    } catch (account_not_found_exception &e) {
        accounts.emplace_back(&db, account_name);
    }
}

void bank::delete_account(const std::string &account_name) {
    auto account_it = find_account(account_name);
    if (account_it->delete_account()) {
        accounts.erase(account_it);
    } else throw std::runtime_error("Can't delete account: " + account_name);
}

bank::accounts_container<account>::iterator bank::find_account(const std::string &account_name) {
    if (auto it = std::find(accounts.begin(), accounts.end(), account_name); it != accounts.end()) {
        return it;
    }
    throw account_not_found_exception();
}

void bank::restore_data_from_db() {
    std::vector<std::string> accounts_from_db = db.get_all_accounts();
    for (auto &account_name : accounts_from_db) {
        accounts.emplace_back(&db, account_name);
        std::vector<std::string> account_plans = db.get_all_account_plans(account_name);
        for (auto &plan_name : account_plans) {
            accounts.back().create_plan(plan_name);
        }
    }
}
