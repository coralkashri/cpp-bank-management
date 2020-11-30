#include "plan.h"

std::string plan::get_plan_name() const {
    return plan_name;
}

bool plan::operator==(const std::string &name) {
    return plan_name == name;
}

bool plan::operator!=(const std::string &name) {
    return !(*this == name);
}
