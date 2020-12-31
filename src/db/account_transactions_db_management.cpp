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
                                                                       : db_ptr(db_ptr), accounts_db_m(accounts_db_m) {}

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

bool account_transactions_db_management::is_transaction_exists(const std::string &account_name, const transaction_id &id) const {
    // DB desired table access
    mongocxx::collection accounts_table = (*db_ptr)[accounts_db_m->get_accounts_table_name()];

    // Build filter
    document filter = build_find_transaction_filter(account_name, id);

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
    bsoncxx::builder::basic::document filter = build_find_transaction_filter(account_name, id);

    // Prepare update
    bsoncxx::builder::basic::document update;
    update.append(kvp("$set", [is_active](sub_document $set) {
        $set.append(kvp("transactions.$.is_active", is_active));
    }));

    // Perform update
    accounts_table.update_one(filter.view(), update.view());
}

void account_transactions_db_management::update_account_transaction(const std::string &account_name,
                                                                    const transaction &transaction_data) {
    // DB desired table access
    mongocxx::collection accounts_table = (*db_ptr)[accounts_db_m->get_accounts_table_name()];

    // Validations
    if (!accounts_db_m->is_account_exists(account_name)) throw account_not_found_exception();
    if (!is_transaction_exists(account_name, transaction_data.id)) throw transaction_not_found_exception(transaction_data.id);

    // Prepare filter
    bsoncxx::builder::basic::document filter = build_find_transaction_filter(account_name, transaction_data.id);

    // Prepare update
    bsoncxx::builder::basic::document update;
    update.append(kvp("$set", [&transaction_data](sub_document $set) {
        $set.append(kvp("transactions.$.cash", transaction_data.cash));
        $set.append(kvp("transactions.$.is_single_time", transaction_data.is_single_time));
        $set.append(kvp("transactions.$.is_active", transaction_data.is_active));
    }));

    // Update options
    auto update_options = mongocxx::options::update();
    /*auto array_filter = build_transaction_update_array_filter(transaction_data.id);
    update_options.array_filters(array_filter.view());
    std::cout << bsoncxx::to_json(update_options.array_filters()->view()) << std::endl;*/

    // Perform update
    accounts_table.update_one(filter.view(), update.view(), update_options);
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
    update.append(kvp("$push", [&transaction_data](sub_document $push) {
        $push.append(kvp("transactions", [&transaction_data](sub_document new_transaction) {
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

bsoncxx::builder::basic::document account_transactions_db_management::build_find_transaction_filter(const std::string &account_name,
                                                                                                    const transaction_id &t_id) const {
    // Prepare filter
    bsoncxx::builder::basic::document filter;
    filter.append(kvp("account_name", account_name));
    /*filter.append(kvp("transactions.transaction_name_id", t_id.transaction_name));
    filter.append(kvp("transactions.is_income", t_id.is_income));*/

    filter.append(kvp("transactions", [t_id](sub_document transactions) {
        transactions.append(kvp("$elemMatch", [&](sub_document $elem_match) {
            $elem_match.append(kvp("transaction_name_id", [&](sub_document transaction_name_id) {
                transaction_name_id.append(kvp("$eq", t_id.transaction_name));
            }));
            $elem_match.append(kvp("is_income", [&](sub_document is_income) {
                is_income.append(kvp("$eq", t_id.is_income));
            }));
        }));
    }));

    return filter;
}

bsoncxx::v_noabi::array::value account_transactions_db_management::build_transaction_update_array_filter(const transaction_id &t_id) const {
    using bsoncxx::builder::stream::open_array;
    using bsoncxx::builder::stream::close_array;
    using bsoncxx::builder::stream::open_document;
    using bsoncxx::builder::stream::close_document;
    using bsoncxx::builder::stream::finalize;

    auto arr = bsoncxx::builder::stream::array()
            << open_document
                << "$and" << open_array
                    << open_document
                        << "transactions.transaction_name_id" << open_document
                            << "$eq" << t_id.transaction_name
                        << close_document
                    << close_document << open_document
                        << "transactions.is_income" << open_document
                            << "$eq" << t_id.is_income
                        << close_document
                    << close_document
                << close_array
            << close_document << finalize;

    return arr;
}