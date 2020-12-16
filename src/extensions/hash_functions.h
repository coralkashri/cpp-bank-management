/**
 * Origin:
 * https://stackoverflow.com/a/37007715/8038186
 */

#ifndef BANKMANAGEMENT_HASH_FUNCTIONS_H
#define BANKMANAGEMENT_HASH_FUNCTIONS_H

#include <iostream>
#include <vector>
#include <array>

namespace my_utils {
    namespace hash_impl {
        namespace details {
            namespace adl {
                template<class T>
                std::size_t hash(T const &t) {
                    return std::hash<T>{}(t);
                }

                /// ------------------------- Specializations for base types goes here -------------------------
                template <typename T>
                inline std::size_t hash(keys_management<T> const &t) {
                    return std::hash<std::string>{}(t.get_description());
                }
            }
            template<class T>
            std::size_t hasher(T const& t) {
                using adl::hash;
                return hash(t);
            }
        }
        struct hash_tag {};
        template<class T>
        std::size_t hash(hash_tag, T const& t) {
            return details::hasher(t);
        }
        template<class T>
        std::size_t hash_combine(hash_tag, std::size_t seed, T const& t) {
            seed ^= hash(t) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        template<class Container>
        std::size_t fash_hash_random_container(hash_tag, Container const& c ) {
            std::size_t size = c.size();
            std::size_t stride = 1 + size/10;
            std::size_t r = hash(hash_tag{}, size);
            for (auto it = c.begin(); it != c.end(); std::advance(it, stride)) {
                r = hash_combine(hash_tag{}, r, *it);
            }
            return r;
        }

        /// ---------------------- std specializations for collections goes here ----------------------
        template<class T, class A>
        std::size_t hash(hash_tag, std::vector<T,A> const& v) {
            return fash_hash_random_container(hash_tag{}, v);
        }
        template<class T, std::size_t N>
        std::size_t hash(hash_tag, std::array<T,N> const& a) {
            return fash_hash_random_container(hash_tag{}, a);
        }
        template<class T>
        std::size_t hash(hash_tag, std::initializer_list<T> const& il) {
            return fash_hash_random_container(hash_tag{}, il);
        }
        // etc
    }
    struct my_hasher {
        template<class T>
        std::size_t operator()(T const& t)const {
            return hash_impl::hash(hash_impl::hash_tag{}, t);
        }
    };
}

#endif //BANKMANAGEMENT_HASH_FUNCTIONS_H