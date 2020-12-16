#include "plans_db_management.h"
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

plans_db_management::plans_db_management(mongocxx::database *db_ptr) : db_ptr(db_ptr) {
    plans_table_name = "plans";
}

void plans_db_management::create_plan(const std::string &account_name, const std::string &plan_name) {
    // DB desired table access
    mongocxx::collection plans_table = (*db_ptr)[plans_table_name];

    // Validations
    if (is_plan_exists(account_name, plan_name)) throw plan_already_exists_exception();

    // Insert plan
    auto builder = bsoncxx::builder::stream::document{};
    bsoncxx::document::value doc_value = builder
            << "account_name" << account_name
            << "plan_name" << plan_name
            << "cash" << 0.
            << bsoncxx::builder::stream::finalize;
    bsoncxx::stdx::optional<mongocxx::result::insert_one> result = plans_table.insert_one(doc_value.view());
}

double plans_db_management::delete_plan(const std::string &account_name, const std::string &plan_name) {
    // DB desired table access
    mongocxx::collection plans_table = (*db_ptr)[plans_table_name];

    // Validations
    if (!is_plan_exists(account_name, plan_name)) throw plan_not_found_exception();

    // Collect cash
    double current_plan_cash = get_plan_balance(account_name, plan_name);

    // Perform delete
    plans_table.delete_one(document{}
                                   << "account_name" << account_name
                                   << "plan_name" << plan_name
                                   << finalize);

    return current_plan_cash;
}

void plans_db_management::modify_plan_balance(const std::string &account_name, const std::string &plan_name, double cash) {
    // DB desired table access
    mongocxx::collection plans_table = (*db_ptr)[plans_table_name];

    // Validations
    if (!is_plan_exists(account_name, plan_name)) throw plan_not_found_exception();

    // Prepare update
    auto filter = document{}
            << "account_name" << account_name
            << "plan_name" << plan_name
            << finalize;

    auto update = document{}
            << "$inc"
            << open_document
            << "cash" << cash
            << close_document
            << finalize;

    // Perform update
    plans_table.update_one(filter.view(), update.view());
}

double plans_db_management::get_plan_balance(const std::string &account_name, const std::string &plan_name) const {
    // DB desired table access
    mongocxx::collection plans_table = (*db_ptr)[plans_table_name];

    // Validations
    if (!is_plan_exists(account_name, plan_name)) throw plan_not_found_exception();

    // Get balance
    bsoncxx::stdx::optional<bsoncxx::document::value> plan =
            plans_table.find_one(document{}
                                         << "account_name" << account_name
                                         << "plan_name" << plan_name
                                         << finalize);
    if (plan) {
        bsoncxx::document::element cash_elem = plan->view()["cash"];
        if (cash_elem.type() != bsoncxx::type::k_double)
            throw table_field_type_matching_exception();
        return cash_elem.get_double().value;
    }
    throw std::runtime_error("Get plan balance -> plan found but error occurred when accessing it.");
}

bool plans_db_management::is_plan_exists(const std::string &account_name, const std::string &plan_name) const {
    // DB desired table access
    mongocxx::collection plans_table = (*db_ptr)[plans_table_name];

    // Perform check
    bsoncxx::stdx::optional<bsoncxx::document::value> maybe_result =
            plans_table.find_one(document{}
                                         << "account_name" << account_name
                                         << "plan_name" << plan_name
                                         << finalize);
    return (bool) maybe_result;
}

std::vector<std::string> plans_db_management::get_all_account_plans(const std::string &account_name) const {
    // DB desired table access
    mongocxx::collection plans_table = (*db_ptr)[plans_table_name];

    // Perform query
    std::vector<std::string> plan_names;
    mongocxx::cursor cursor = plans_table.find(document{}
                                                       << "account_name"
                                                       << open_document
                                                       << "$eq" << account_name
                                                       << close_document
                                                       << finalize);
    for (auto doc : cursor) {
        bsoncxx::document::element plan_elem = doc["plan_name"];
        if (plan_elem.type() != bsoncxx::type::k_utf8)
            throw table_field_type_matching_exception();
        plan_names.emplace_back(plan_elem.get_utf8().value.to_string());
    }
    return plan_names;
}

void plans_db_management::delete_all_account_plans(const std::string &account_name) {
    // DB desired table access
    mongocxx::collection plans_table = (*db_ptr)[plans_table_name];

    // Perform delete
    bsoncxx::stdx::optional<mongocxx::result::delete_result> result =
            plans_table.delete_many(document{}
                                            << "account_name"
                                            << open_document
                                            << "$eq" << account_name
                                            << close_document
                                            << finalize);

    if (result) {
        std::cout << "Amount of deleted plans: " << result->deleted_count() << "\n";
    }
}