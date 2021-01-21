#include <string>
#include <vector>
#include <array>

template <char f = 'a', size_t K = 26>
struct trie {
	struct node {
		bool leaf;
		std::array<int, K> next;
		node() : leaf(false) {
			next.fill(-1);
		}
	};

	std::vector<node> t;

	trie() : t(1) {}

	void insert(const std::string &s) {
		int cur = 0;
		for (char ch : s) {
			if (t[cur].next[ch - f] == -1) {
				t[cur].next[ch - f] = (int) t.size();
				t.emplace_back();
			}
			cur = t[cur].next[ch - f];
		}
		t[cur].leaf = true;
	}

	bool search(const std::string &s) {
		int cur = 0;
		for (char ch : s) {
			if (t[cur].next[ch - f] == -1) {
				return false;
			}
			cur = t[cur].next[ch - f];
		}
		return t[cur].leaf;
	}

	node &operator[](int i) {
		return t[i];
	}
};