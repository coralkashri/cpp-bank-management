#ifndef BANKMANAGEMENT_OUTPUT_LOGGER_MANAGER_H
#define BANKMANAGEMENT_OUTPUT_LOGGER_MANAGER_H

#include <vector>
#include <memory>
#include "../extensions/custom_concepts.h"

class output_logger_manager {
private:
    enum class printing_state : bool {
        OFF = false,
        ON = true
    };

public:
    output_logger_manager() : output(std::cout), state(printing_state::ON), null_output(nullptr) {}

    void turn_off_printing_state() { state = printing_state::OFF; }
    void turn_on_printing_state() { state = printing_state::ON; }

    std::ostream& printer() {
        return state == printing_state::ON ? output : null_output;
    }

private:
    std::ostream& output;
    std::ostream null_output;
    printing_state state;
};

#endif //BANKMANAGEMENT_OUTPUT_LOGGER_MANAGER_H