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

accounts_db_management::accounts_db_management(mongocxx::database *db_ptr) : db_ptr(db_ptr) {
    accounts_table_name = "accounts";
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
    builder.append(kvp("monthly_income", [](sub_array monthly_income) {
        /*monthly_income.append([](sub_document elem) {
            elem.append(kvp("income_source", "name"));
            elem.append(kvp("income_cash", 0.));
            elem.append(kvp("is_single_time", false));
        });*/
    }));
    builder.append(kvp("monthly_outcome", [](sub_array monthly_outcome) {
        /*monthly_outcome.append([](sub_document elem) {
            elem.append(kvp("outcome_name", "name"));
            elem.append(kvp("outcome_cash", 0.));
            elem.append(kvp("is_single_time", false));
        });*/
    }));
    builder.append(kvp("archive", [](sub_array archive) {
        /*archive.append([](sub_document elem) {
            elem.append(kvp("month", "01.12.2020")); // December
            elem.append(kvp("monthly_income", [](sub_array monthly_income) {
                monthly_income.append([](sub_document elem) {
                    elem.append(kvp("income_source", "name"));
                    elem.append(kvp("income_cash", 0.));
                    elem.append(kvp("is_single_time", false));
                });
            }));
            elem.append(kvp("monthly_outcome", [](sub_array monthly_outcome) {
                monthly_outcome.append([](sub_document elem) {
                    elem.append(kvp("outcome_name", "name"));
                    elem.append(kvp("outcome_cash", 0.));
                    elem.append(kvp("is_single_time", false));
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

void accounts_db_management::update_account_monthly_income(const std::string &account_name,
                                                           const std::string &income_source_name,
                                                           double income) {
    // DB desired table access
    mongocxx::collection accounts_table = (*db_ptr)[accounts_table_name];

    // Validations
    if (!is_account_exists(account_name)) throw account_not_found_exception();

    // Prepare filter
    bool income_exists = is_income_exists(account_name, income_source_name);
    bsoncxx::builder::basic::document filter;
    filter.append(kvp("account_name", account_name));
    if (income_exists) {
        filter.append(kvp("monthly_income.income_source", income_source_name));
    }

    // Prepare update
    bsoncxx::builder::basic::document update;
    if (income_exists) {
        update.append(kvp("$set", [income](sub_document $set) {
            $set.append(kvp("monthly_income.$.income_cash", income));
            $set.append(kvp("monthly_income.$.is_single_time", false));
        }));
    } else {
        update.append(kvp("$push", [income, income_source_name](sub_document $push) {
            $push.append(kvp("monthly_income", [income, income_source_name](sub_document monthly_income) {
                monthly_income.append(kvp("income_source", income_source_name));
                monthly_income.append(kvp("income_cash", income));
                monthly_income.append(kvp("is_single_time", false));
            }));
        }));
    }

    // Perform update
    accounts_table.update_one(filter.view(), update.view());
}

void accounts_db_management::update_account_monthly_outcome(const std::string &account_name,
                                                            const std::string &outcome_name,
                                                            double outcome_cash) {
}

void accounts_db_management::add_account_single_time_outcome(const std::string &account_name,
                                                             const std::string &outcome_name,
                                                             double outcome_cash) {
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

bool accounts_db_management::is_income_exists(const std::string &account_name, const std::string &income_source_name) const {
    // DB desired table access
    mongocxx::collection accounts_table = (*db_ptr)[accounts_table_name];

    document filter;
    filter.append(kvp("account_name", account_name));
    filter.append(kvp("monthly_income.income_source", income_source_name));

    // Perform search
    auto result = accounts_table.find_one(filter.view());
    return (bool)result;
}