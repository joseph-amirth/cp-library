#pragma once

#include <vector>
#include <functional>

namespace hashing {

template <typename Key, typename Value>
struct naive_perfect_hash_table {
    using key_type = Key;
    using value_type = Value;

    int m;
    std::function<int(key_type)> f;
    std::vector<value_type> table;

    naive_perfect_hash_table() {}

    template <typename Generator>
    naive_perfect_hash_table(std::vector<key_type> &keys, Generator &&generator) {
        if (!keys.empty()) {
            m = 32 - __builtin_clz(keys.size() * keys.size());
            table.resize(1 << m);

            std::vector<key_type *> used(1 << m, nullptr);
            while (f = generator(m), true) {
                bool invalid = false;
                fill(used.begin(), used.end(), nullptr);
                for (auto &key : keys) {
                    auto hash_value = f(key);
                    if (used[hash_value] != nullptr && *used[hash_value] != key) {
                        invalid = true;
                        break;
                    }
                    used[hash_value] = &key;
                }
                if (!invalid) {
                    break;
                }
            }
        }
    }

    value_type &operator[](const key_type &key) {
        return table[f(key)];
    }

    const value_type &operator[](const key_type &key) const {
        return table[f(key)];
    }
};

template <typename Key, typename Value>
struct perfect_hash_table {
    using key_type = Key;
    using value_type = Value;

    int m;
    std::function<int(key_type)> f;
    std::vector<naive_perfect_hash_table<key_type, value_type>> table;

    perfect_hash_table() {}

    template <typename Generator>
    perfect_hash_table(const std::vector<key_type> &keys, Generator &&generator) {
        m = 32 - __builtin_clz(keys.size());
        table.resize(1 << m);

        std::vector<int> count(1 << m);
        while (f = generator(m), true) {
            long long square_sum = 0;
            fill(count.begin(), count.end(), 0);
            for (const auto &key : keys) {
                auto hash_value = f(key);
                square_sum += 2 * count[hash_value] + 1;
                count[hash_value] += 1;
            }
            if (square_sum < (2 << m)) {
                break;
            }
        }

        std::vector<std::vector<key_type>> bins(1 << m);
        for (int i = 0; i < (1 << m); i++) {
            bins[i].reserve(count[i]);
        }

        for (const auto &key : keys) {
            bins[f(key)].push_back(key);
        }

        for (int i = 0; i < (1 << m); i++) {
            table[i] = naive_perfect_hash_table<key_type, value_type>(bins[i], generator);
        }
    }

    value_type &operator[](const key_type &key) {
        return table[f(key)][key];
    }

    const value_type &operator[](const key_type &key) const {
        return table[f(key)][key];
    }
};

}
