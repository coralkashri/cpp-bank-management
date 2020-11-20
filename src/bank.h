#ifndef BANKMANAGEMENT_BANK_H
#define BANKMANAGEMENT_BANK_H

#include <vector>
#include <map>
#include <string>
#include "account.h"

class bank {
public:
    bank();

    std::vector<std::string> get_accounts_names() const;
    account get_account(std::string account_name);
    bool create_account(std::string name);
    void delete_account(std::string name);

private:
    std::vector<account> accounts;
};

#endif //BANKMANAGEMENT_BANK_H