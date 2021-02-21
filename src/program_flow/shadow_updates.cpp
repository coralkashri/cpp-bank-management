#include "shadow_updates.h"
#include "structures/transactions.h"
#include <iostream>

shadow_updates::shadow_updates()
    : monthly_cash_updater_t([this] {
        monthly_cash_updater(*this);
    }) {
}

shadow_updates::~shadow_updates() {
    monthly_cash_updater_t.join();
}

void shadow_updates::monthly_cash_updater(shadow_updates &ref) {
    ref.wait_for_update_time();
    auto accounts = ref.db.get_all_accounts();
    auto current_time = boost::gregorian::date(boost::gregorian::day_clock::local_day_ymd());
    for (auto const &account : accounts) {
        auto account_incomes = transactions(ref.db.get_account_income_details(account, current_time));
        auto account_outcomes = transactions(ref.db.get_account_outcome_details(account, current_time));

        // Single Time Transactions Handler
        auto single_time_incomes = account_incomes.get_transactions_by(transactions::IS_SINGLE_TIME, true);
        auto single_time_outcomes = account_outcomes.get_transactions_by(transactions::IS_SINGLE_TIME, true);
        // TODO


        // Continues Transactions Handler
        auto continues_time_incomes = account_incomes.get_transactions_by(transactions::IS_SINGLE_TIME, false);
        auto continues_time_outcomes = account_outcomes.get_transactions_by(transactions::IS_SINGLE_TIME, false);

        // Active Transactions Handler
        auto active_incomes = account_incomes.get_transactions_by(transactions::IS_ACTIVE, true);
        auto active_outcomes = account_outcomes.get_transactions_by(transactions::IS_ACTIVE, true);

        // Inactive Transactions Handler
        auto inactive_incomes = account_incomes.get_transactions_by(transactions::IS_ACTIVE, false);
        auto inactive_outcomes = account_outcomes.get_transactions_by(transactions::IS_ACTIVE, false);
    }
}

bool shadow_updates::is_month_updated() const {
    return false; // TODO
}

void shadow_updates::wait_for_update_time() const {
    if (is_month_updated()) {
        std::this_thread::sleep_for(std::chrono::hours(6));
    }
}
