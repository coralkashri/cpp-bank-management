/**
 * Origin:
 * https://github.com/koralkashri/multimap
 */

#ifndef BANKMANAGEMENT_KEYS_MANAGEMENT_H
#define BANKMANAGEMENT_KEYS_MANAGEMENT_H

#include <vector>
#include <string>
#include <type_traits>
#include <ranges>
#include <functional>

struct keys_management_default_print;

template <typename KEY_T, typename TO_STR = keys_management_default_print>
class keys_management {
private:
    friend TO_STR;
    std::function<std::ostream&(std::ostream& out, const keys_management<KEY_T, TO_STR> &ref)> print_function = TO_STR();

public:
    keys_management(const keys_management<KEY_T, TO_STR>&) = default;
    keys_management(keys_management<KEY_T, TO_STR>&&) = default;
    keys_management& operator=(const keys_management<KEY_T, TO_STR>&) = default;
    keys_management& operator=(keys_management<KEY_T, TO_STR>&&) = default;

    keys_management(const std::initializer_list<KEY_T>& keys, const std::string &description = "")
            : description(description) {
        auto filtered_keys = keys | std::views::filter([] (auto &key) { return !key.empty(); }) | std::views::common;
        this->keys.insert(this->keys.begin(), filtered_keys.begin(), filtered_keys.end());
    }

    bool operator==(const KEY_T &ref) const {
        auto it = std::find(keys.begin(), keys.end(), ref);
        return it != keys.end();
    }

    bool operator<(const KEY_T &ref) const {
        auto local_min = find_relative_key(keys, std::less());
        return local_min < ref;
    }

    bool operator>(const KEY_T &ref) const {
        auto local_max = find_relative_key(keys, std::greater());
        return local_max > ref;
    }

    bool operator==(const keys_management<KEY_T, TO_STR> &ref) const {
        for (auto &key : ref.keys) {
            if (key == *this) return true;
        }
        return false;
    }

    bool operator<(const keys_management<KEY_T, TO_STR> &ref) const {
        auto ref_min = find_relative_key(ref.keys, std::less());
        return *this < ref_min;
    }

    bool operator!=(const KEY_T &ref) const {
        return !(*this == ref);
    }

    friend std::ostream& operator<<(std::ostream& out, const keys_management<KEY_T, TO_STR> &ref) {
        return ref.print_function(out, ref);
    }

    [[nodiscard]] std::string get_description() const {return description; }

    [[nodiscard]] std::vector<std::string> get_keys() const {return keys; }

private:
    std::vector<KEY_T> keys;
    std::string description;

    template <typename Op_T>
    KEY_T find_relative_key(const std::vector<KEY_T> &keys_vec, Op_T op) const {
        auto it = keys_vec.begin();
        auto rel_key = *it;
        for (std::advance(it, 1); it != keys_vec.end(); std::advance(it, 1)) {
            if (op(rel_key, *it)) rel_key = *it;
        }
        return rel_key;
    }
};

struct keys_management_default_print {
    template <typename KEY_T>
    std::ostream& operator()(std::ostream& out, const keys_management<KEY_T, keys_management_default_print> &keys_management_ref) {
        out << "[";
        for (auto &key : keys_management_ref.keys) {
            out << key;
            if (&key != &keys_management_ref.keys.back()) {
                out << ", ";
            }
        }
        out << "]";
        if (!keys_management_ref.description.empty()) {
            out << " - " << keys_management_ref.description;
        }
        return out;
    }
};

#endif //BANKMANAGEMENT_KEYS_MANAGEMENT_H