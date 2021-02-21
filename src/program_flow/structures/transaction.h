#ifndef BANKMANAGEMENT_TRANSACTION_H
#define BANKMANAGEMENT_TRANSACTION_H

#include <string>
#include <vector>
#include "../../extensions/ranges_extensions.h"

struct transaction_id {
    std::string transaction_name;
    bool is_income;

    [[nodiscard]] bool operator==(const transaction_id &t_id) const {
        return is_income == t_id.is_income && transaction_name == t_id.transaction_name;
    }
};

struct transaction {
    transaction_id id;
    double cash;
    bool is_single_time;
    bool is_active;

    [[nodiscard]] bool operator==(const transaction_id &t_id) const {
        return id == t_id;
    }
};

std::ostream& operator<<(std::ostream& out, const transaction_id &t_id);
std::ostream& operator<<(std::ostream& out, const transaction &t);

#endif //BANKMANAGEMENT_TRANSACTION_H