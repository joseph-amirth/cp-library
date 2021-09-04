namespace splay_tree {
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

	template <typename splay_node, typename key_t>
	bool find(splay_node **root, const key_t &key) {
		splay_node *temp = *root;
		while (temp != nullptr) {
			if (temp->key == key) {
				break;
			} else if (key < temp->key) {
				temp = temp->left;
			} else {
				temp = temp->right;
			}
		}
		if (temp != nullptr) {
			splay(temp);
			*root = temp;
			return true;
		}
		return false;
	}

	template <typename splay_node, typename key_t>
	void lower_bound(splay_node **root, const key_t &key) {
		splay_node *temp = *root, *last_greater = nullptr;
		while (temp != nullptr) {
			if (temp->key == key) {
				splay(temp), last_greater = nullptr;
				break;
			} else if (key < temp->key) {
				last_greater = temp;
				temp = temp->left;
			} else {
				temp = temp->right;
			}
		}
		if (last_greater != nullptr) {
			splay(last_greater);
			*root = last_greater;
		}
	}

	template <typename splay_node, typename key_t>
	void upper_bound(splay_node **root, const key_t &key) {
		splay_node *temp = *root, *last_greater = nullptr;
		while (temp != nullptr) {
			if (key < temp->key) {
				last_greater = temp;
				temp = temp->left;
			} else {
				temp = temp->right;
			}
		}
		if (last_greater != nullptr) {
			splay(last_greater);
			*root = last_greater;
		}
	}

	template <typename splay_node>
	void find_minimum(splay_node **root) {
		if (*root != nullptr) {
			splay_node *temp = *root;
			while (temp->left != nullptr) {
				temp = temp->left;
			}
			splay(temp);
			*root = temp;
		}
	}

	template <typename splay_node>
	void find_maximum(splay_node **root) {
		if (*root != nullptr) {
			splay_node *temp = *root;
			while (temp->right != nullptr) {
				temp = temp->right;
			}
			splay(temp);
			*root = temp;
		}
	}

	template <typename splay_node>
	bool find_kth_key(splay_node **root, std::size_t k) {
		if (*root == nullptr || k >= (*root)->size) {
			return false;
		}
		splay_node *temp = *root;
		while (temp != nullptr) {
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
		*root = temp;
		return true;
	}

	template <typename splay_node, typename key_t>
	std::size_t order_of_key(splay_node **root, const key_t &key) {
		splay_node *temp = *root;
		std::size_t order = 0;
		while (temp != nullptr) {
			if (key < temp->key) {
				temp = temp->left;
			} else {
				order += (temp->left != nullptr ? temp->left->size : 0);
				temp = temp->right;
			}
		}
		return order;
	}

	template <typename splay_node, typename key_t>
	splay_node* split(splay_node **root, const key_t &key) {
		if (*root != nullptr) {
			upper_bound(&*root, key);
			if ((*root)->key > key) {
				splay_node *other = *root;
				*root = (*root)->left;
				if (*root != nullptr) {
					(*root)->parent = nullptr;
				}
				other->left = nullptr;
				other->pull_up();
				return other;
			} else {
				return nullptr;
			}
		}
		return nullptr;
	}

	template <typename splay_node>
	splay_node* join(splay_node *root, splay_node *other) {
		if (root == nullptr) {
			return other;
		} else if (other == nullptr) {
			return root;
		} else {
			find_maximum(&root);
			root->right = other;
			other->parent = root;
			root->pull_up();
			return root;
		}
	}

	template <typename splay_node, typename key_t>
	void insert(splay_node **root, const key_t &key) {
		if (*root == nullptr) {
			*root = new splay_node(key);
		} else {
			splay_node *temp = *root, *parent = nullptr;
			while (temp != nullptr) {
				if (temp->key == key) {
					splay(temp);
					*root = temp;
					return;
				}
				parent = temp;
				if (key < temp->key) {
					temp = temp->left;
				} else {
					temp = temp->right;
				}
			}
			splay_node *new_root;
			if (key < parent->key) {
				parent->left = new splay_node(key);
				new_root = parent->left;
			} else {
				parent->right = new splay_node(key);
				new_root = parent->right;
			}
			new_root->parent = parent;
			splay(new_root);
			*root = new_root;
		}
	}

	template <typename splay_node, typename key_t>
	void erase(splay_node **root, const key_t &key) {
		if (find(&*root, key)) {
			splay_node *temp = *root;
			if (temp->left != nullptr) {
				temp->left->parent = nullptr;
			}
			if (temp->right != nullptr) {
				temp->right->parent = nullptr;
			}
			*root = join(temp->left, temp->right);
			delete temp;
		}
	}
}