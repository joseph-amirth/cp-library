#include <random>
#include <chrono>

namespace treap {
	std::mt19937_64 rng(std::chrono::steady_clock::now().time_since_epoch().count());

	struct node {
		int key, size, priority;
		node *l, *r;
		node() : key(), size(), priority(), l(nullptr), r(nullptr) {}
		node(int key, int priority) : key(key), priority(priority) {
			size = 1;
			l = r = nullptr;
		}
	};

	int size(node *t) {
		return t != nullptr ? t->size : 0;
	}

	void split(node *t, int key, node *&l, node *&r) {
		if (t == nullptr) {
			l = r = nullptr;
		} else if (key < t->key) {
			split(t->l, key, l, t->l);
			r = t;
		} else {
			split(t->r, key, t->r, r);
			l = t;
		}
	}

	void insert(node *&t, node *it) {
		if (!t) {
			t = it;
		} else if (t->priority < it->priority) {
			split(t, it->key, it->l, it->r);
			t = it;
		} else {
			insert(t->key < it->key ? t->l : t->r, it);
		}
	}

	void merge(node *&t, node *l, node *r) {
		if (l == nullptr) {
			t = r;
		} else if (r == nullptr) {
			t = l;
		} else if (l->priority > r->priority) {
			merge(l->r, l->r, r);
			t = l;
		} else {
			merge(r->l, l, r->l);
			t = r;
		}
	}

	void erase(node *&t, int key) {
		if (t->key == key) {
			node *temp = t;
			merge(t, t->l, t->r);
			delete temp;
		} else {
			erase(key < t->key ? t->l : t->r, key);
		}
	}
}



