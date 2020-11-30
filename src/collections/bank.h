#ifndef BANKMANAGEMENT_BANK_H
#define BANKMANAGEMENT_BANK_H

#include <vector>
#include <map>
#include <string>
#include "account.h"

class bank {
public:
    bank(const std::string &db_path);

    [[nodiscard]] std::vector<std::string> get_account_names() const;
    account& get_account(const std::string &account_name);
    void create_account(const std::string &name);
    void delete_account(const std::string &name);

private:
    template <typename T>
    using accounts_container = std::vector<T>;
    accounts_container<account> accounts;
    std::string db_path;

    accounts_container<account>::iterator find_account(const std::string &name);
};

#endif //BANKMANAGEMENT_BANK_H