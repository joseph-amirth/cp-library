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

template <typename HeadCharSet, typename...TailCharSets>
struct char_set_union {
    using head_char_set_type = HeadCharSet;
    using tail_char_sets_union_type = char_set_union<TailCharSets...>;
    using char_type = typename head_char_set_type::char_type;

    static constexpr int size() {
        return head_char_set_type::size() + tail_char_sets_union_type::size();
    }

    static constexpr int index_of(char_type ch) {
        if (head_char_set_type::contains(ch)) {
            return head_char_set_type::index_of(ch);
        } else {
            return head_char_set_type::size() + tail_char_sets_union_type::index_of(ch);
        }
    }

    static constexpr char_type char_at(int i) {
        if (i < head_char_set_type::size()) {
            return head_char_set_type::char_at(i);
        } else {
            return tail_char_sets_union_type::char_at(i - head_char_set_type::size());
        }
    }

    static constexpr bool contains(char_type c) {
        return head_char_set_type::contains(c) || tail_char_sets_union_type::contains(c);
    }
};

template <typename Char, Char...chars>
using char_set = char_set_union<char_singleton<Char, chars>...>;

template <typename CharSet>
struct char_set_union<CharSet> : CharSet {};

using ascii = char_range<char, 0, 1 << 7>;
using digits = char_range<char, '0', 10>;
using lowercase_latin_alphabet = char_range<char, 'a', 26>;
using uppercase_latin_alphabet = char_range<char, 'A', 26>;
using latin_alphabet = char_set_union<lowercase_latin_alphabet, uppercase_latin_alphabet>;

}
