#include "accounts_db_management.h"
#include <bsoncxx/json.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
#include <bsoncxx/builder/stream/helpers.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/array.hpp>
#include "../extensions/custom_exceptions.h"

using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;

accounts_db_management::accounts_db_management(mongocxx::database *db_ptr) : db_ptr(db_ptr) {
    accounts_table_name = "accounts";
}

void accounts_db_management::create_account(const std::string &account_name) {
    // DB desired table access
    mongocxx::collection accounts_table = (*db_ptr)[accounts_table_name];

    // Validations
    if (is_account_exists(account_name)) throw account_alreasy_exists();

    // Insert account
    auto builder = bsoncxx::builder::stream::document{};
    bsoncxx::document::value doc_value = builder
            << "account_name" << account_name
            << "free_cash" << 0.
            << bsoncxx::builder::stream::finalize;
    bsoncxx::stdx::optional<mongocxx::result::insert_one> result = accounts_table.insert_one(doc_value.view());
}

void accounts_db_management::delete_account(const std::string &account_name) {
    // DB desired table access
    mongocxx::collection accounts_table = (*db_ptr)[accounts_table_name];

    // Validations
    if (!is_account_exists(account_name)) throw account_not_found_exception();

    // Perform delete
    accounts_table.delete_one(document{} << "account_name" << account_name << finalize);
    /* todo delete_all_account_plans(account_name);*/
}

double accounts_db_management::get_account_free_cash(const std::string &account_name) {
    // DB desired table access
    mongocxx::collection accounts_table = (*db_ptr)[accounts_table_name];

    // Validations
    if (!is_account_exists(account_name)) throw account_not_found_exception();

    // Get balance
    bsoncxx::stdx::optional<bsoncxx::document::value> account =
            accounts_table.find_one(document{}
                                         << "account_name" << account_name
                                         << finalize);
    if (account) {
        bsoncxx::document::element cash_elem = account->view()["free_cash"];
        if (cash_elem.type() != bsoncxx::type::k_double)
            throw table_field_type_matching_exception();
        return cash_elem.get_double().value;
    }
    throw std::runtime_error("Get account free cash -> account found but error occurred when accessing it.");
}

std::vector<std::string> accounts_db_management::get_all_accounts() {
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

bool accounts_db_management::is_account_exists(const std::string &account_name) {
    // DB desired table access
    mongocxx::collection accounts_table = (*db_ptr)[accounts_table_name];

    // Perform check
    bsoncxx::stdx::optional<bsoncxx::document::value> maybe_result = accounts_table.find_one(
            document{} << "account_name" << account_name << finalize);
    return (bool) maybe_result;
}
