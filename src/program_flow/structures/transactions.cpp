#include "transactions.h"
#include <numeric>

void transactions::initialize_query_field_by_filter_type() {
    query_field_by_filter_type = {
            {IS_ACTIVE, &transaction::is_active},
            {IS_SINGLE_TIME, &transaction::is_single_time},
    };
}

double transactions::sum() const {
    auto cash_list = trancs | std::views::transform([](auto const &tranc) { return tranc.cash; }) | std::views::common;
    return std::accumulate(cash_list.begin(), cash_list.end(), 0, std::plus<>());
}

transactions transactions::get_transactions_by(transactions::transaction_filter_types transaction_filter_type, bool condition) const {
    if (transaction_filter_type == IS_INCOME) {
        return trancs | std::views::filter([condition](auto const &tranc) { return tranc.id.is_income == condition; }) | std::views::common;
    } else {
        return trancs | std::views::filter([condition, transaction_filter_type, this](auto const &tranc) {
            return tranc.*(query_field_by_filter_type.at(transaction_filter_type)) == condition;
        }) | std::views::common;
    }
}