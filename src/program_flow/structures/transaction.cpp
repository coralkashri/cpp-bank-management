#include "transaction.h"
#include <iostream>

std::ostream& operator<<(std::ostream& out, const transaction_id &t_id) {
    out <<  (t_id.is_income ? "Income" : "Outcome") <<  " name: " << t_id.transaction_name << "\n";
    return out;
}

std::ostream& operator<<(std::ostream& out, const transaction &t) {
    out <<  t.id << "\n" <<
        "Cash: " << t.cash << "\n" <<
       (t.is_single_time ? "Single time" : "Repeated") << "\n" <<
       (t.is_active ? "Active" : "Paused") << "\n";
    return out;
}