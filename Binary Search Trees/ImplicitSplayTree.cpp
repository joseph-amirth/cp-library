#include <cassert>
#include <iterator>

namespace implicit_splay_tree {
	template <typename splay_node>
	void right_rotate(splay_node *x, splay_node *y) {
		y->left = x->right;
		x->right = y;
		if (y->left != nullptr) {
			y->left->parent = y;
		}
		x->parent = y->parent;
		y->parent = x;
		if (x->parent != nullptr) {
			if (x->parent->left == y) {
				x->parent->left = x;
			} else {
				x->parent->right = x;
			}
		}
	}

	template <typename splay_node>
	void left_rotate(splay_node *x, splay_node *y) {
		y->right = x->left;
		x->left = y;
		if (y->right != nullptr) {
			y->right->parent = y;
		}
		x->parent = y->parent;
		y->parent = x;
		if (x->parent != nullptr) {
			if (x->parent->left == y) {
				x->parent->left = x;
			} else {
				x->parent->right = x;
			}
		}
	}

	template <typename splay_node>
	void splay(splay_node *x) {
		splay_node *y, *z;
		while (y = x->parent, y != nullptr) {
			if (z = y->parent, z == nullptr) {
				if (y->left == x) {
					right_rotate(x, y);
				} else {
					left_rotate(x, y);
				}
				y->pull_up();
				x->pull_up();
			} else {
				if (z->left == y && y->left == x) {
					right_rotate(y, z);
					right_rotate(x, y);
				} else if (z->right == y && y->right == x) {
					left_rotate(y, z);
					left_rotate(x, y);
				} else if (z->left == y && y->right == x) {
					left_rotate(x, y);
					right_rotate(x, z);
				} else {
					right_rotate(x, y);
					left_rotate(x, z);
				}
				z->pull_up();
				y->pull_up();
				x->pull_up();
			}
		}
	}

	template <typename splay_node>
	splay_node* find_kth(splay_node *root, std::size_t k) {
		assert(root != nullptr && k < root->size);
		splay_node *temp = root;
		while (temp != nullptr) {
			temp->push_down();
			if (temp->left != nullptr && temp->left->size > k) {
				temp = temp->left;
			} else {
				if (temp->left != nullptr) {
					k -= temp->left->size;
				}
				if (k == 0) {
					break;
				} else {
					k -= 1;
					temp = temp->right;
				}
			}
		}
		splay(temp);
		return temp;
	}

	template <typename splay_node>
	std::size_t order_of(splay_node *x) {
		assert(x != nullptr);
		std::size_t order = (x->left != nullptr ? x->left->size : 0);
		while (x->parent != nullptr) {
			if (x->parent->right == x) {
				order += x->parent->size - x->right->size;
			}
			x = x->parent;
		}
		return order;
	}

	template <typename splay_node>
	splay_node* find_first(splay_node *root) {
		return find_kth(root, 0);
	}

	template <typename splay_node>
	splay_node* find_last(splay_node *root) {
		assert(root != nullptr);
		return find_kth(root, root->size - 1);
	}

	template <typename splay_node>
	splay_node* split(splay_node **root, std::size_t k) {
		*root = find_kth(*root, k);
		splay_node *other = (*root)->right;
		(*root)->right = nullptr;
		if (other != nullptr) {
			other->parent = nullptr;
		}
		(*root)->pull_up();
		return other;
	}

	template <typename splay_node>
	splay_node* join(splay_node *root, splay_node *other) {
		if (root == nullptr) {
			return other;
		} else if (other == nullptr) {
			return root;
		} else {
			root = find_last(root);
			root->right = other;
			other->parent = root;
			root->pull_up();
			return root;
		}
	}

	template <typename splay_node, typename...constructor_args>
	splay_node* insert(splay_node *root, std::size_t k, constructor_args...args) {
		assert(k <= (root == nullptr ? 0 : root->size));
		splay_node *new_root = new splay_node(args...);
		if (root == nullptr) {

		} else if (k == 0) {
			new_root->right = root;
			root->parent = new_root;
		} else if (k == root->size) {
			new_root->left = root;
			root->parent = new_root;
		} else {
			splay_node *other = split(&root, k - 1);
			new_root->left = root;
			root->parent = new_root;
			new_root->right = other;
			other->parent = new_root;
		}
		new_root->pull_up();
		return new_root;
	}

	template <typename splay_node>
	splay_node* erase(splay_node *root, std::size_t k) {
		root = find_kth(root, k);
		splay_node *temp = root;
		if (temp->left != nullptr) {
			temp->left->parent = nullptr;
		}
		if (temp->right != nullptr) {
			temp->right->parent = nullptr;
		}
		root = join(temp->left, temp->right);
		delete temp;
		return root;
	}

	template <typename splay_node, typename operation>
	splay_node* query(splay_node *root, std::size_t l, std::size_t r, operation &&f) {
		assert(root != nullptr && l <= r && r < root->size);
		splay_node *prefix = nullptr, *middle = root, *suffix = nullptr;
		if (r + 1 != middle->size) {
			suffix = split(&middle, r);
		}
		if (l != 0) {
			prefix = middle;
			middle = split(&prefix, l - 1);
		}
		f(middle);
		return join(join(prefix, middle), suffix);
	}

	template <typename splay_node, typename iterator_t>
	splay_node* build_tree(iterator_t first, iterator_t last) {
		if (first == last) {
			return nullptr;
		} else {
			auto mid = std::next(first, std::distance(first, last) / 2);
			splay_node *root = new splay_node(*mid);
			root->left = build_tree<splay_node>(first, mid);
			if (root->left != nullptr) {
				root->left->parent = root;
			}
			root->right = build_tree<splay_node>(std::next(mid), last);
			if (root->right != nullptr) {
				root->right->parent = root;
			}
			root->pull_up();
			return root;
		}
	}

	template <typename splay_node, typename operation>
	void visit_inorder(splay_node *root, operation &&f) {
		if (root != nullptr) {
			visit_inorder(root->left, f);
			f(root);
			visit_inorder(root->right, f);
		}
	}

	template <typename splay_node, typename operation>
	void visit_preorder(splay_node *root, operation &&f) {
		if (root != nullptr) {
			f(root);
			visit_preorder(root->left, f);
			visit_preorder(root->right, f);
		}
	}

	template <typename splay_node, typename operation>
	void visit_postorder(splay_node *root, operation &&f) {
		if (root != nullptr) {
			visit_postorder(root->left, f);
			visit_postorder(root->right, f);
			f(root);
		}
	}

	template <typename splay_node>
	void delete_tree(splay_node *root) {
		if (root != nullptr) {
			if (root->parent != nullptr) {
				if (root->parent->left == root) {
					root->parent->left = nullptr;
				} else {
					root->parent->right = nullptr;
				}
			}
			visit_postorder(root, [](splay_node *x) {
				delete x;
			});
		}
	}
}