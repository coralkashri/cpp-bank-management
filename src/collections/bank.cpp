#include "bank.h"
#include "../extensions/custom_exceptions.h"

bank::bank(const std::string &db_path) : db_path(db_path) {}

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

void bank::create_account(const std::string &name) {
    try {
        find_account(name);
        throw account_alreasy_exists();
    } catch (account_not_found_exception &e) {
        accounts.emplace_back(db_path, name);
    }
}

void bank::delete_account(const std::string &name) {
    auto account_it = find_account(name);
    accounts.erase(account_it);
}

bank::accounts_container<account>::iterator bank::find_account(const std::string &name) {
    if (auto it = std::find(accounts.begin(), accounts.end(), name); it != accounts.end()) {
        return it;
    }
    throw account_not_found_exception();
}
