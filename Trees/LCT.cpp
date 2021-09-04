#include "../Binary Search Trees/ImplicitSplayTree.cpp"
#include <vector>

struct lct_node {
	int vertex, size;
	lct_node *parent, *left, *right;

	lct_node *path_parent;
	bool flipped;

	lct_node(int u) : vertex(u) {
		size = 1;
		parent = left = right = nullptr;

		path_parent = nullptr;
		flipped = false;
	}

	void push_down() {
		if (left != nullptr) {
			left->path_parent = path_parent;
			left->flipped ^= flipped;
		}
		if (right != nullptr) {
			right->path_parent = path_parent;
			right->flipped ^= flipped;
		}
		if (flipped) {
			std::swap(left, right);
			flipped = false;
		}
	}
};

struct link_cut_tree {
	int n;
	std::vector<lct_node*> nodes;

	link_cut_tree(int n) : nodes(n) {
		for (int i = 0; i < n; i++) {
			nodes[i] = new lct_node(i);
		}
	}

	void access(int u) {
		implicit_splay_tree::splay(nodes[u]);
		if (nodes[u]->right != nullptr) {
			nodes[u]->right->parent = nullptr;
			nodes[u]->right->path_parent = nodes[u];
			nodes[u]->right = nullptr;
		}
		lct_node *now = nodes[u];
		while (now->path_parent != nullptr) {
			int v = now->path_parent->vertex;
			implicit_splay_tree::splay(nodes[v]);
			if (nodes[v]->right != nullptr) {
				nodes[v]->right->parent = nullptr;
				nodes[v]->right->path_parent = nodes[v];
			}
			now->path_parent = nodes[v]->path_parent;
			nodes[v]->right = implicit_splay_tree::find_first(now);
			nodes[v]->right->parent = nodes[v];
			now = nodes[v];
		}
	}

	int find(int u) {
		access(u);
		implicit_splay_tree::splay(nodes[u]);
		return implicit_splay_tree::find_first(nodes[u])->vertex;
	}

	void link(int u, int v) {
		access(u);
		implicit_splay_tree::splay(nodes[u]);
		nodes[u]->flipped ^= true;
		access(v);
		nodes[v]->right = nodes[u];
		nodes[u]->parent = nodes[v];
	}

	void cut(int u, int v) {

	}

	int path(int v);

	~link_cut_tree() {
		for (lct_node *node : nodes) {
			delete node;
		}
	}
};