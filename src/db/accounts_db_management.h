#ifndef BANKMANAGEMENT_ACCOUNTS_DB_MANAGEMENT_H
#define BANKMANAGEMENT_ACCOUNTS_DB_MANAGEMENT_H

#include <string>
#include <vector>
#include <mongocxx/instance.hpp>
#include <mongocxx/client.hpp>
#include "account_transactions_db_management.h"

class accounts_db_management {
public:
    explicit accounts_db_management(mongocxx::database *db_ptr);

    void create_account(const std::string &account_name);

    void delete_account(const std::string &account_name);

    void modify_free_cash(const std::string &account_name, double cash);

    [[nodiscard]] std::vector<std::string> get_all_accounts() const;

    [[nodiscard]] bool is_account_exists(const std::string &account_name) const;

    [[nodiscard]] double get_account_free_cash(const std::string &account_name) const;

    [[nodiscard]] std::string get_accounts_table_name() const;

    [[nodiscard]] account_transactions_db_management get_transactions_management() const;

private:
    std::string accounts_table_name;
    mongocxx::database *db_ptr;
    account_transactions_db_management transactions_management;
};

#endif //BANKMANAGEMENT_ACCOUNTS_DB_MANAGEMENT_H