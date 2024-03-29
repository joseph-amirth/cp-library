#pragma once

#include <array>
#include <vector>
#include <bitset>
#include <algorithm>

template<int N>
struct basic_sieve {
    std::bitset<N + 1> is_prime;

    basic_sieve() : is_prime() {
        is_prime.set(), is_prime[0] = is_prime[1] = false;
        for (int i = 2; i * i <= N; i++) {
            if (is_prime[i]) {
                for (int j = i * i; j <= N; j += i) {
                    is_prime[j] = false;
                }
            }
        }
    }
};

template<int N>
struct spf_sieve {
    std::vector<int> primes;
    std::array<int, N + 1> spf;

    spf_sieve() : spf() {
        for (int i = 2; i <= N; i++) {
            if (spf[i] == 0) {
                spf[i] = i;
                primes.push_back(i);
            }
            for (int j = 0; j < (int) primes.size() && i * primes[j] <= N; j++) {
                spf[i * primes[j]] = primes[j];
                if (i % primes[j] == 0) {
                    break;
                }
            }
        }
    }

    bool is_prime(int x) const {
        return spf[x] == x;
    }

    std::vector<std::pair<int, int>> prime_factors(int x) const {
        std::vector<std::pair<int, int>> factors;
        while (x != 1) {
            if (factors.empty() || factors.back().first != spf[x]) {
                factors.emplace_back(spf[x], 0);
            }
            factors.back().second++;
            x /= spf[x];
        }
        return factors;
    }

    std::vector<int> divisors(int x) const {
        std::vector<int> divisors = {1};
        while (x > 1) {
            int p = spf[x], c = 0;
            while (x % p == 0) {
                x /= p, c += 1;
            }
            int sz = (int) divisors.size();
            for (int i = 1, pw = p; i <= c; i++, pw *= p) {
                for (int j = 0; j < sz; j++) {
                    divisors.push_back(divisors[j] * pw);
                }
            }
        }
        return divisors;
    }

    template<typename T, typename F>
    T compute_multiplicative_function(int x, F &&f) const {
        T result = T(1);
        while (x > 1) {
            int p = spf[x], c = 0;
            while (spf[x] == p) {
                x /= p, c += 1;
            }
            result *= f(p, c);
        }
        return result;
    }
};