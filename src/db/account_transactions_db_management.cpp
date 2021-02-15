#include "account_transactions_db_management.h"
#include <bsoncxx/json.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
#include <bsoncxx/builder/stream/helpers.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/array.hpp>
#include "../extensions/custom_exceptions.h"
#include "accounts_db_management.h"

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::sub_array;
using bsoncxx::builder::basic::sub_document;
using bsoncxx::builder::basic::document;
using bsoncxx::builder::basic::array;

account_transactions_db_management::account_transactions_db_management(mongocxx::database *db_ptr, accounts_db_management *accounts_db_m)
               : db_ptr(db_ptr), accounts_db_m(accounts_db_m), transactions_db_field_name("transactions"),
               transactions_filter(transactions_db_field_name) {
}

void account_transactions_db_management::update_account_monthly_income(const std::string &account_name,
                                                                       const std::string &source_name,
                                                                       double income) {

    update_or_create_account_transaction(account_name, transaction{
            .id = {
                    .transaction_name = source_name,
                    .is_income = true
            },
            .cash = income,
            .is_single_time = false,
            .is_active = true
    });
}

void account_transactions_db_management::set_account_single_time_income(const std::string &account_name,
                                                                        const std::string &source_name,
                                                                        double income) {

    update_or_create_account_transaction(account_name, transaction{
            .id = {
                    .transaction_name = source_name,
                    .is_income = true
            },
            .cash = income,
            .is_single_time = true,
            .is_active = false
    });
}

void account_transactions_db_management::pause_account_monthly_income(const std::string &account_name, const std::string &source_name) {
    change_account_transaction_state(account_name, transaction_id{
            .transaction_name = source_name,
            .is_income = true
    }, false);
}

void account_transactions_db_management::restart_account_monthly_income(const std::string &account_name, const std::string &source_name) {
    change_account_transaction_state(account_name, transaction_id{
            .transaction_name = source_name,
            .is_income = true
    }, true);
}

void account_transactions_db_management::update_account_monthly_outcome(const std::string &account_name,
                                                                        const std::string &target_name,
                                                                        double outcome) {

    update_or_create_account_transaction(account_name, transaction{
            .id = {
                    .transaction_name = target_name,
                    .is_income = false
            },
            .cash = outcome,
            .is_single_time = false,
            .is_active = true
    });
}

void account_transactions_db_management::set_account_single_time_outcome(const std::string &account_name,
                                                                         const std::string &target_name,
                                                                         double outcome) {

    update_or_create_account_transaction(account_name, transaction{
            .id = {
                    .transaction_name = target_name,
                    .is_income = false
            },
            .cash = outcome,
            .is_single_time = true,
            .is_active = false
    });
}

void account_transactions_db_management::pause_account_monthly_outcome(const std::string &account_name, const std::string &target_name) {
    change_account_transaction_state(account_name, transaction_id{
            .transaction_name = target_name,
            .is_income = true
    }, false);
}

void account_transactions_db_management::restart_account_monthly_outcome(const std::string &account_name, const std::string &target_name) {
    change_account_transaction_state(account_name, transaction_id{
            .transaction_name = target_name,
            .is_income = false
    }, true);
}

std::vector<transaction> account_transactions_db_management::get_account_outcome_details(const std::string &account_name,
                                                                const boost::gregorian::date &month) const {
    return get_account_transactions_details(account_name, month, false);
}

std::vector<transaction> account_transactions_db_management::get_account_income_details(const std::string &account_name,
                                                                                        const boost::gregorian::date &month) const {
    return get_account_transactions_details(account_name, month, true);
}


bool account_transactions_db_management::is_transaction_exists(const std::string &account_name, const transaction_id &id) const {
    // DB desired table access
    mongocxx::collection accounts_table = (*db_ptr)[accounts_db_m->get_accounts_table_name()];

    // Build filter
    document filter;
    filter.append(kvp("account_name", account_name));
    expanded_filter.apply<db_account_transactions_filter::ID>(transactions_filter, filter, "", id);

    // Perform search
    auto result = accounts_table.find_one(filter.view());
    return (bool)result;
}

void account_transactions_db_management::update_or_create_account_transaction(const std::string &account_name,
                                                                              const transaction &transaction_data) {
    // Validations
    if (!accounts_db_m->is_account_exists(account_name)) throw account_not_found_exception();

    bool is_transaction_name_id_exists = is_transaction_exists(account_name, transaction_data.id);
    if (is_transaction_name_id_exists) {
        update_account_transaction(account_name, transaction_data);
    } else {
        create_account_transaction(account_name, transaction_data);
    }
}

void account_transactions_db_management::change_account_transaction_state(const std::string &account_name,
                                                                          const transaction_id &id,
                                                                          bool is_active) {
    // DB desired table access
    mongocxx::collection accounts_table = (*db_ptr)[accounts_db_m->get_accounts_table_name()];

    // Validations
    if (!accounts_db_m->is_account_exists(account_name)) throw account_not_found_exception();
    if (!is_transaction_exists(account_name, id)) throw transaction_not_found_exception(id);

    // Prepare filter
    document filter;
    filter.append(kvp("account_name", account_name));
    expanded_filter.apply<db_account_transactions_filter::ID>(transactions_filter, filter, "", id);

    // Prepare update
    document update;
    update.append(kvp("$set", [is_active, this](sub_document $set) {
        $set.append(kvp(transactions_db_field_name + ".$.is_active", is_active));
    }));

    // Perform update
    accounts_table.update_one(filter.view(), update.view());
}

std::vector<transaction> account_transactions_db_management::get_account_transactions_details(const std::string &account_name,
                                                                     const boost::gregorian::date &month,
                                                                     bool is_income) const {
    // Relevant variables
    std::vector<transaction> relevant_transactions;
    bool is_archive = boost::gregorian::day_clock::local_day_ymd().month != month.month();
    std::string transactions_field_hierarchy = is_archive ? "archive." : "";

    // DB desired table access
    mongocxx::collection accounts_table = (*db_ptr)[accounts_db_m->get_accounts_table_name()];

    // Validations
    if (!accounts_db_m->is_account_exists(account_name)) throw account_not_found_exception();
    if (boost::gregorian::day_clock::local_day_ymd().month < month.month()) throw illegal_date_exception();

    // Prepare filter

    // Get transactions from archive
    mongocxx::pipeline filter{};
    document is_income_aggregate_filter;
    transactions_filter.apply<db_account_transactions_filter::AGR_IS_INCOME>(is_income_aggregate_filter, transactions_field_hierarchy, is_income);
    filter.match(bsoncxx::builder::basic::make_document(kvp("account_name", account_name)));
    filter.project(is_income_aggregate_filter.view());
    if (is_archive) {
        document archive_aggregate_filter;
        transactions_filter.apply<db_account_transactions_filter::AGR_ARCHIVE_BY_DATE>(archive_aggregate_filter, "", month);
        filter.project(archive_aggregate_filter.view());
    }
    auto accounts = accounts_table.aggregate(filter, mongocxx::options::aggregate{});
    for (auto account : accounts) {
        auto desired_transactions = account[transactions_field_hierarchy + transactions_db_field_name];
        for (auto &t : desired_transactions.get_array().value) {
            auto &current_transaction = relevant_transactions.emplace_back();
            auto actual_transaction = t.get_document().value;
            current_transaction.id = {
                .transaction_name = actual_transaction["transaction_name_id"].get_utf8().value.to_string(),
                .is_income = actual_transaction["is_income"].get_bool().value
            };
            current_transaction.cash = actual_transaction["cash"].get_double().value;
            current_transaction.is_single_time = actual_transaction["is_single_time"].get_bool().value;
            current_transaction.is_active = actual_transaction["is_active"].get_bool().value;
        }
    }
    return relevant_transactions;
}

void account_transactions_db_management::update_account_transaction(const std::string &account_name,
                                                                    const transaction &transaction_data) {
    // DB desired table access
    mongocxx::collection accounts_table = (*db_ptr)[accounts_db_m->get_accounts_table_name()];

    // Validations
    if (!accounts_db_m->is_account_exists(account_name)) throw account_not_found_exception();
    if (!is_transaction_exists(account_name, transaction_data.id)) throw transaction_not_found_exception(transaction_data.id);

    // Prepare filter
    document filter;
    filter.append(kvp("account_name", account_name));
    expanded_filter.apply<db_account_transactions_filter::ID>(transactions_filter, filter, "", transaction_data.id);

    // Prepare update
    bsoncxx::builder::basic::document update;
    update.append(kvp("$set", [&transaction_data, this](sub_document $set) {
        $set.append(kvp(transactions_db_field_name + ".$.cash", transaction_data.cash));
        $set.append(kvp(transactions_db_field_name + ".$.is_single_time", transaction_data.is_single_time));
        $set.append(kvp(transactions_db_field_name + ".$.is_active", transaction_data.is_active));
    }));

    // Perform update
    accounts_table.update_one(filter.view(), update.view());
}

void account_transactions_db_management::create_account_transaction(const std::string &account_name,
                                                                    const transaction &transaction_data) {
    // DB desired table access
    mongocxx::collection accounts_table = (*db_ptr)[accounts_db_m->get_accounts_table_name()];

    // Validations
    if (!accounts_db_m->is_account_exists(account_name)) throw account_not_found_exception();
    if (is_transaction_exists(account_name, transaction_data.id)) throw transaction_already_exists_exception(transaction_data.id);

    // Prepare filter
    bsoncxx::builder::basic::document filter;
    filter.append(kvp("account_name", account_name));

    // Prepare update
    bsoncxx::builder::basic::document update;
    update.append(kvp("$push", [&transaction_data, this](sub_document $push) {
        $push.append(kvp(transactions_db_field_name, [&transaction_data](sub_document new_transaction) {
            new_transaction.append(kvp("transaction_name_id", transaction_data.id.transaction_name));
            new_transaction.append(kvp("is_income", transaction_data.id.is_income));
            new_transaction.append(kvp("cash", transaction_data.cash));
            new_transaction.append(kvp("is_single_time", transaction_data.is_single_time));
            new_transaction.append(kvp("is_active", !transaction_data.is_single_time));
        }));
    }));

    // Perform update
    accounts_table.update_one(filter.view(), update.view());
}

std::string account_transactions_db_management::get_date_for_db(const boost::gregorian::date &date) {
    std::stringstream ss;
    ss << "01." <<
       date.month() << "." <<
       date.year();
    return ss.str();
}
