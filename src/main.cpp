#include <iostream>
#include "program_flow/program_action.h"

int main() {
    std::string db_path; // todo read from program options
    program_action pa(std::move(db_path));
    pa.run();
    return 0;
}