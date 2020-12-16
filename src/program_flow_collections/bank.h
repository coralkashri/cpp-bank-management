#ifndef BANKMANAGEMENT_BANK_H
#define BANKMANAGEMENT_BANK_H

#include <vector>
#include <map>
#include <string>
#include "account.h"
#include "../db/db_management.h"
#include "../structures/output_logger_manager.h"

class bank {
public:
    bank(output_logger_manager *output);

    [[nodiscard]] std::vector<std::string> get_account_names() const;
    account& get_account(const std::string &account_name);
    void create_account(const std::string &account_name);
    void delete_account(const std::string &account_name);

private:
    template <typename T>
    using accounts_container = std::vector<T>;
    accounts_container<account> accounts;
    db_management db;
    output_logger_manager *output;

    accounts_container<account>::iterator find_account(const std::string &account_name);

    void restore_data_from_db();
};

#endif //BANKMANAGEMENT_BANK_H