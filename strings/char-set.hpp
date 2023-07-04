#pragma once

namespace strings {

template <typename Char, Char ch>
struct char_singleton {
    using char_type = Char;

    static constexpr int size() {
        return 1;
    }

    static constexpr int index_of(char_type) {
        return 0;
    }

    static constexpr Char char_at(int) {
        return ch;
    }

    static constexpr bool contains(char_type c) {
        return c == ch;
    }
};

template <typename Char, Char f, int K>
struct char_range {
    using char_type = Char;

    static constexpr int size() {
        return K;
    }

    static constexpr int index_of(char_type c) {
        return c - f;
    }

    static constexpr Char char_at(int i) {
        return i + f;
    }

    static constexpr bool contains(char_type c) {
        return f <= c && c < f + K;
    }
};

template <typename...CharSets>
struct char_set_union {
    using char_type = char;

    static constexpr int size() {
        return (CharSets::size() + ... + 0);
    }

    static constexpr int index_of(char_type ch) {
        int index = 0;
        ((CharSets::contains(ch) ? (index += CharSets::index_of(ch), false) : (index += CharSets::size(), true)) && ...);
        return index;
    }

    static constexpr char_type char_at(int i) {
        char_type answer;
        ((i < CharSets::size() ? (answer = CharSets::char_at(i), false) : (i -= CharSets::size(), true)) && ...);
        return answer;
    }

    static constexpr bool contains(char_type c) {
        return (CharSets::contains(c) || ... || false);
    }
};

template <typename Char, Char...chars>
using char_set = char_set_union<char_singleton<Char, chars>...>;

using ascii = char_range<char, 0, 1 << 7>;
using digits = char_range<char, '0', 10>;
using lowercase_latin_alphabet = char_range<char, 'a', 26>;
using uppercase_latin_alphabet = char_range<char, 'A', 26>;
using latin_alphabet = char_set_union<lowercase_latin_alphabet, uppercase_latin_alphabet>;

}
