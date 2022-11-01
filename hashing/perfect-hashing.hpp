#pragma once

#include <vector>
#include <functional>

namespace hashing {

template <typename key_t, typename value_t>
struct perfect_hash_table {
    int m;
    std::function<int(key_t)> f;
    std::vector<value_t> table;

    perfect_hash_table() {}

    template <typename generator_t>
    perfect_hash_table(std::vector<key_t> &keys, generator_t &&generator) {
        if (!keys.empty()) {
            m = 32 - __builtin_clz(keys.size() * keys.size());
            table.resize(1 << m);

            std::vector<key_t *> used(1 << m, nullptr);
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

    value_t &operator[](const key_t &key) {
        return table[f(key)];
    }

    const value_t &operator[](const key_t &key) const {
        return table[f(key)];
    }
};

template <typename key_t, typename value_t>
struct linear_perfect_hash_table {
    int m;
    std::function<int(key_t)> f;
    std::vector<perfect_hash_table<key_t, value_t>> table;

    linear_perfect_hash_table() {}

    template <typename generator_t>
    linear_perfect_hash_table(const std::vector<key_t> &keys, generator_t &&generator) {
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

        std::vector<std::vector<key_t>> bins(1 << m);
        for (int i = 0; i < (1 << m); i++) {
            bins[i].reserve(count[i]);
        }

        for (const auto &key : keys) {
            bins[f(key)].push_back(key);
        }

        for (int i = 0; i < (1 << m); i++) {
            table[i] = perfect_hash_table<key_t, value_t>(bins[i], generator);
        }
    }

    value_t &operator[](const key_t &key) {
        return table[f(key)][key];
    }

    const value_t &operator[](const key_t &key) const {
        return table[f(key)][key];
    }
};

}