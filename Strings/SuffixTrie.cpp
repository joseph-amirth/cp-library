#include "Trie.h"

template <char f = 'a', size_t K = 26>
struct suffix_trie : public trie<f, K> {
	suffix_trie(const std::string &s) {
		for (int i = 0; i < s.length(); i++) {
			trie<f, K>::insert(s.substr(i, s.length()));
		}
	}
};