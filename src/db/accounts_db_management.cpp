#include "accounts_db_management.h"
#include <bsoncxx/json.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
#include <bsoncxx/builder/stream/helpers.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/array.hpp>
#include "../extensions/custom_exceptions.h"

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::sub_array;
using bsoncxx::builder::basic::sub_document;
using bsoncxx::builder::basic::document;
using bsoncxx::builder::basic::array;

accounts_db_management::accounts_db_management(mongocxx::database *db_ptr)
: db_ptr(db_ptr), accounts_table_name("accounts"), transactions_management(db_ptr, this) {
}

void accounts_db_management::create_account(const std::string &account_name) {
    // DB desired table access
    mongocxx::collection accounts_table = (*db_ptr)[accounts_table_name];

    // Validations
    if (is_account_exists(account_name)) throw account_alreasy_exists();

    // Insert account
    document builder;
    builder.append(kvp("account_name", account_name));
    builder.append(kvp("free_cash", 0.));
    builder.append(kvp("transactions", [](sub_array transactions) {
        /*transactions.append([](sub_document new_transaction) {
            new_transaction.append(kvp("transaction_name_id", "name"));
            new_transaction.append(kvp("is_income", true));
            new_transaction.append(kvp("cash", 0.));
            new_transaction.append(kvp("is_single_time", false));
            new_transaction.append(kvp("is_active", true));
        });*/
    }));
    builder.append(kvp("archive", [](sub_array archive) {
        /*archive.append([](sub_document elem) {
            elem.append(kvp("month", "01.12.2020")); // December
            elem.append(kvp("transactions", [](sub_array transactions) {
                transactions.append([](sub_document new_transaction) {
                    new_transaction.append(kvp("transaction_name_id", "name"));
                    new_transaction.append(kvp("is_income", true));
                    new_transaction.append(kvp("income_cash", 0.));
                    new_transaction.append(kvp("is_single_time", false));
                    new_transaction.append(kvp("is_active", true));
                });
            }));
        });*/
    }));
    bsoncxx::stdx::optional<mongocxx::result::insert_one> result = accounts_table.insert_one(builder.view());
}

void accounts_db_management::delete_account(const std::string &account_name) {
    // DB desired table access
    mongocxx::collection accounts_table = (*db_ptr)[accounts_table_name];

    // Validations
    if (!is_account_exists(account_name)) throw account_not_found_exception();

    // Perform delete
    accounts_table.delete_one(bsoncxx::builder::basic::make_document(kvp("account_name", account_name)).view());
}

void accounts_db_management::modify_free_cash(const std::string &account_name, double cash) {
    // DB desired table access
    mongocxx::collection accounts_table = (*db_ptr)[accounts_table_name];

    // Validations
    if (!is_account_exists(account_name)) throw account_not_found_exception();

    // Prepare filter
    document filter;
    filter.append(kvp("account_name", account_name));

    // Prepare update
    document update;
    update.append(kvp("$inc", [cash](sub_document $inc) {
        $inc.append(kvp("free_cash", cash));
    }));

    // Perform update
    accounts_table.update_one(filter.view(), update.view());
}

double accounts_db_management::get_account_free_cash(const std::string &account_name) const {
    // DB desired table access
    mongocxx::collection accounts_table = (*db_ptr)[accounts_table_name];

    // Validations
    if (!is_account_exists(account_name)) throw account_not_found_exception();

    // Get balance
    bsoncxx::stdx::optional<bsoncxx::document::value> account =
            accounts_table.find_one(bsoncxx::builder::basic::make_document(kvp("account_name", account_name)));

    if (account) {
        bsoncxx::document::element cash_elem = account->view()["free_cash"];
        if (cash_elem.type() != bsoncxx::type::k_double)
            throw table_field_type_matching_exception();
        return cash_elem.get_double().value;
    }
    throw std::runtime_error("Get account free cash -> account found but error occurred when accessing it.");
}

std::vector<std::string> accounts_db_management::get_all_accounts() const {
    // DB desired table access
    mongocxx::collection accounts_table = (*db_ptr)[accounts_table_name];

    // Perform query
    std::vector<std::string> account_names;
    mongocxx::cursor cursor = accounts_table.find({});
    for (auto doc : cursor) {
        bsoncxx::document::element account_elem = doc["account_name"];
        if (account_elem.type() != bsoncxx::type::k_utf8)
            throw table_field_type_matching_exception();
        account_names.emplace_back(account_elem.get_utf8().value.to_string());
    }
    return account_names;
}

bool accounts_db_management::is_account_exists(const std::string &account_name) const {
    // DB desired table access
    mongocxx::collection accounts_table = (*db_ptr)[accounts_table_name];

    // Perform check
    bsoncxx::stdx::optional<bsoncxx::document::value> maybe_result = accounts_table.find_one(
            bsoncxx::builder::basic::make_document(kvp("account_name", account_name)));
    return (bool) maybe_result;
}

std::string accounts_db_management::get_accounts_table_name() const {
    return accounts_table_name;
}

account_transactions_db_management accounts_db_management::get_transactions_management() const {
    return transactions_management;
}
