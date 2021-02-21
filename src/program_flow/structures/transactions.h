#ifndef BANKMANAGEMENT_TRANSACTIONS_H
#define BANKMANAGEMENT_TRANSACTIONS_H

#include <map>
#include <ranges>
#include "transaction.h"

template <typename T>
concept TransactionsContainer = requires (T t) {
    { std::vector<transaction>(t.begin(), t.end()) };
};

struct transactions {
private:
    template <typename T>
    using inner_cont = std::vector<T>;

public:
    enum transaction_filter_types {
        IS_ACTIVE,
        IS_INCOME,
        IS_SINGLE_TIME,
    };

public:
    template <TransactionsContainer Cont>
    transactions(Cont &&trancs) : trancs(trancs.begin(), trancs.end()) {
        initialize_query_field_by_filter_type();
    }

    template <TransactionsContainer Cont>
    transactions& operator=(Cont &&trancs_cont) {
        this->trancs = std::vector<transaction>(trancs_cont.begin(), trancs_cont.end());
        initialize_query_field_by_filter_type();
        return *this;
    }

    [[nodiscard]] double sum() const;

    [[nodiscard]] transactions get_transactions_by(transaction_filter_types transaction_filter_type, bool condition) const;

    transaction& at(transaction_id &&t_id) {
        auto it = std::find(trancs.begin(), trancs.end(), t_id);
        if (it == trancs.end()) throw std::runtime_error("Transaction not found in transactions collection.");
        return *it;
    }

private:
    inner_cont<transaction> trancs;
    std::map<transaction_filter_types, bool(transaction::*)> query_field_by_filter_type;

    transactions() = default;

    void initialize_query_field_by_filter_type();
};


#endif //BANKMANAGEMENT_TRANSACTIONS_H
