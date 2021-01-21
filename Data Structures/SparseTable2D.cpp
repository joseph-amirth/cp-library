#include <functional>
#include <cmath>

#include "../Custom Datatype/Tensor.cpp"

template<typename T>
struct sparse_table_2d {
	using F = std::function<T(const T&, const T&)>;

	int n, m;
	tensor::dynamic_tensor<T, 4> mat;
	F f;

	sparse_table_2d(): n(), m(), f() {}

	template<typename U>
	sparse_table_2d(const U &arr, int n, int m, F f): n(n), m(m), f(f), mat(int(log2(n)) + 1, int(log2(m)) + 1) {
		mat[0][0].assign(n, m);
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++) {
				mat[0][0][i][j] = T(arr[i][j]);
			}
		}

		for (int k = 1; k < mat.size(); k++) {
			mat[k][0].assign(n - (1 << k) + 1, m);
			for (int i = 0; i + (1 << k) <= n; i++) {
				for (int j = 0; j < m; j++) {
					mat[k][0][i][j] = f(mat[k - 1][0][i][j], mat[k - 1][0][i + (1 << (k - 1))][j]);
				}
			}
		}

		for (int k = 0; k < mat.size(); k++) {
			for (int l = 1; l < mat[0].size(); l++) {
				mat[k][l].assign(n - (1 << k) + 1, m - (1 << l) + 1);
				for (int i = 0; i + (1 << k) <= n; i++) {
					for (int j = 0; j + (1 << l) <= m; j++) {
						mat[k][l][i][j] = f(mat[k][l - 1][i][j], mat[k][l - 1][i][j + (1 << (l - 1))]);
					}
				}
			}
		}
	}

	T query(int x1, int y1, int x2, int y2) {
		int k = 32 - __builtin_clz(x2 - x1 + 1) - 1, l = 32 - __builtin_clz(y2 - y1 + 1) - 1;
		T res1 = f(mat[k][l][x1][y1], mat[k][l][x2 + 1 - (1 << k)][y1]);
		T res2 = f(mat[k][l][x1][y2 + 1 - (1 << l)], mat[k][l][x2 + 1 - (1 << k)][y2 + 1 - (1 << l)]);
		return f(res1, res2);
	}
};