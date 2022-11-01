#pragma once

#include <string>
#include <vector>
#include <array>

template<typename string_t>
std::vector<int> prefix_function(const string_t &s, int n = -1) {
    if (n == -1) {
        n = (int) s.size();
    }
    std::vector<int> pi(n);
    for (int i = 1; i < n; i++) {
        int j = pi[i - 1];
        while (j > 0 && s[i] != s[j]) {
            j = pi[j - 1];
        }
        if (s[i] == s[j]) {
            pi[i] = j + 1;
        }
    }
    return pi;
}

template<typename string_t>
std::vector<int> kmp(const string_t &s, int n, const string_t &t, int m) {
    std::vector<int> occurrences;
    std::vector<int> lps = prefix_function(t, m);
    for (int i = 0, j = 0; i < n; i++) {
        while (j > 0 && s[i] != t[j]) {
            j = lps[j - 1];
        }
        if (s[i] == t[j]) {
            j += 1;
        }
        if (j == m) {
            occurrences.push_back(i - m + 1);
            j = lps[j - 1];
        }
    }
    return occurrences;
}

std::vector<int> kmp(const std::string &s, const std::string &t) {
    return kmp(s, s.length(), t, t.length());
}

std::vector<int> kmp(const std::vector<int> &s, const std::vector<int> &t) {
    return kmp(s, s.size(), t, t.size());
}

template<typename string_t>
std::vector<int> z_function(const string_t &s, int n = -1) {
    if (n == -1) {
        n = (int) s.size();
    }
    std::vector<int> z(n);
    for (int i = 1, l = 0, r = 0; i < n; i++) {
        z[i] = (i > r ? 0 : std::min(r - i + 1, z[i - l]));
        while (i + z[i] < n && s[z[i]] == s[i + z[i]]) {
            z[i] += 1;
        }
        if (i + z[i] - 1 > r) {
            l = i, r = i + z[i] - 1;
        }
    }
    return z;
}

template <char f ='a', int K = 26, typename string_t>
std::vector<std::vector<int>> prefix_automaton(const string_t &s, int n = -1) {
    if (n == -1) {
        n = (int) s.size();
    }
    std::vector<int> lps = prefix_function(s, n);
    std::vector<std::vector<int>> automaton(n, std::array<int, K>());
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < K; j++) {
            if (i == 0 || j == s[i] - f) {
                automaton[i][j] = i;
            } else {
                automaton[i][j] = automaton[lps[i - 1]][j];
            }
        }
    }
    return automaton;
}