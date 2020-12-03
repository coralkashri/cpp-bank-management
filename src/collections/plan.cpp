#include "plan.h"
#include <iostream>
#include <filesystem>
#include <fstream>

plan::plan(const std::string &db_path, std::string plan_name) : db_path(db_path), plan_name(plan_name) {
    if (auto file_path = (dir_path()/plan_name) += ".bin"; !std::filesystem::is_regular_file(file_path)) {
        std::ofstream new_plan_file(file_path.string(), std::ios::trunc);
        new_plan_file << 0;
    }
}

plan::~plan() {

}

std::string plan::get_plan_name() const {
    return plan_name;
}

bool plan::operator==(const std::string &name) {
    return plan_name == name;
}

bool plan::operator!=(const std::string &name) {
    return !(*this == name);
}

void plan::print_details() const {
    if (auto file_path = (dir_path()/plan_name) += ".bin"; std::filesystem::is_regular_file(file_path)) {
        std::ifstream new_plan_file(file_path.string(), std::ios::trunc);
        double cash;
        new_plan_file.read((char*)&cash, 1);
    }
}

void plan::plan_management() {
    // todo plan_management object
}

bool plan::delete_plan() {
    return false;
}

std::filesystem::path plan::dir_path() {
    return std::filesystem::path();
}
