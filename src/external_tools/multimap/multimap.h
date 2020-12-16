/**
 * Origin:
 * https://github.com/koralkashri/multimap
 */

#ifndef BANKMANAGEMENT_MULTIMAP_H
#define BANKMANAGEMENT_MULTIMAP_H

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include "keys_management.h"
#include "../iterator_helpers/use_collection_iterator.h"
#include "../../extensions/hash_functions.h"

template <typename K, typename V>
using multimap_container_type = std::unordered_map<K, V, my_utils::my_hasher>;

using multimap_key_type = std::string;

template <template <typename ...> typename Container, typename K, typename V>
using my_collection_iterator = use_collection_iterator_without_reverse<Container, K, V>;

template <typename KEY_T, typename VAL_T>
class multimap : public my_collection_iterator<multimap_container_type, multimap_key_type, VAL_T> {
public:
    // A convenient access to inherited type members
    using base_types = my_collection_iterator<multimap_container_type, multimap_key_type, VAL_T>;

public:
    using items_type = multimap_container_type<multimap_key_type, VAL_T>;

private:
    using multikey_map_type = multimap_container_type<keys_management<KEY_T>, VAL_T>;

public:
    multimap() = default;
    multimap(const multikey_map_type &ref) {
        parse_multikey_map_to_multimap(ref);
    }
    multimap& operator=(const multikey_map_type &ref) {
        parse_multikey_map_to_multimap(ref);
        return *this;
    }

    void parse_multikey_map_to_multimap(const multikey_map_type & mkm) {
        for (auto &elem : mkm) {
            auto keys = elem.first.get_keys();
            key_relations.insert(elem.first);
            auto val = elem.second;
            for (auto &key : keys) {
                items.insert({key, val});
            }
        }
    }

    VAL_T& at(const std::string &key) {
        return items.at(key);
    }

    VAL_T at(const std::string &key) const {
        return items.at(key);
    }

    void insert(const keys_management<KEY_T> &keys, VAL_T val) {
        parse_multikey_map_to_multimap(multimap_key_type({keys, val}));
    }

    /* Required Iterators Access Methods */

    typename base_types::iterator begin() { return items.begin(); }
    typename base_types::iterator end() { return items.end(); }
    [[nodiscard]] typename base_types::const_iterator cbegin() const { return items.cbegin(); }
    [[nodiscard]] typename base_types::const_iterator cend() const { return items.cend(); }

    [[nodiscard]] bool empty() const { return items.empty(); }
    [[nodiscard]] typename base_types::size_type size() const { return items.size(); }

    [[nodiscard]] std::set<KEY_T> get_keys() const {
        std::set<KEY_T> res;
        for (auto &keys : key_relations) {
            for (auto &key : keys.get_keys()) {
                res.emplace(key);
            }
        }
        return res;
    }

    [[nodiscard]] std::set<keys_management<KEY_T>> get_key_relations() const { return key_relations; }

private:
    items_type items;
    std::set<keys_management<KEY_T>> key_relations;
};

#endif //BANKMANAGEMENT_MULTIMAP_H