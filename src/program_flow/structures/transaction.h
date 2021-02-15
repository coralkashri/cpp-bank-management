#ifndef BANKMANAGEMENT_TRANSACTION_H
#define BANKMANAGEMENT_TRANSACTION_H

#include <string>

struct transaction_id {
    std::string transaction_name;
    bool is_income;
};

struct transaction {
    transaction_id id;
    double cash;
    bool is_single_time;
    bool is_active;
};

std::ostream& operator<<(std::ostream& out, const transaction_id &t_id);
std::ostream& operator<<(std::ostream& out, const transaction &t);

#endif //BANKMANAGEMENT_TRANSACTION_H