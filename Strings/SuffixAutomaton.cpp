#include <vector>
#include <array>

template <char f = 'a', size_t K = 26>
struct suffix_automaton {
	struct node {
		bool leaf = false;
		int len;
		int link;
		std::array<int, K> next;
		node(int len = 0, int link = -1) : len(len), link(link) {
			next.fill(-1);
		}
	};

	std::vector<node> t;

	suffix_automaton() {}
	suffix_automaton(const std::string &s) : t(1) {
		int last = 0;
		for (char ch : s) {
			last = extend(ch);
		}
		for (int i = last; i != 0; i = t[i].link) {
			t[i].leaf = true;
		}
	}

	int extend(char c) {
		static int last = 0;
		int new_node = int(t.size());
		t.emplace_back(t[last].len + 1);
		int p = last;
		while (p != -1 && t[p].next[c - f] == -1) {
			t[p].next[c - f] = new_node;
			p = t[p].link;
		}
		if (p == -1) {
			t[new_node].link = 0;
		} else {
			int q = t[p].next[c - f];
			if (t[q].len == 1 + t[p].len) {
				t[new_node].link = q;
			} else {
				int clone = int(t.size());
				t.emplace_back(t[q]);
				t[clone].len = t[p].len + 1;
				while (p != -1 && t[p].next[c - f] == q) {
					t[p].next[c - f] = clone;
					p = t[p].link;
				}
				t[q].link = t[new_node].link = clone;
			}
		}
		last = new_node;
		return last;
	}
};