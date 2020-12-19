#ifndef BANKMANAGEMENT_INCOME_SOURCE_H
#define BANKMANAGEMENT_INCOME_SOURCE_H

#include <string>

struct income_source {
    std::string name;
    double income_amount;
    bool is_single_time;
};

using outcome_source = income_source;

#endif //BANKMANAGEMENT_INCOME_SOURCE_H