#include "Matrix.h"

template <typename mint>
int gaussian_elimination(matrix<mint> a, std::vector<mint> &x) {
	int n = int(a.size()), m = int(a[0].size()) - 1;

	std::vector<int> where(m, -1);
	for (int col = 0, row = 0; col < m && row < n; col++) {
		int sel = row;
		for (int i = row; i < n; i++) {
			if (a[i][col] != 0) {
				sel = i;
				break;
			}
		}
		if (a[sel][col] == 0)
			continue;
		for (int i = col; i <= m; i++) {
			swap(a[sel][i], a[row][i]);
		}
		where[col] = row;

		for (int i = 0; i < n; i++) {
			if (i != row) {
				mint c = a[i][col] / a[row][col];
				for (int j = col; j <= m; j++) {
					a[i][j] -= a[row][j] * c;
				}
			}
		}
		++row;
	}

	for (int i = 0; i < m; i++) {
		if (where[i] != -1) {
			x[i] = a[where[i]][m] / a[where[i]][i];
		}
	}

	for (int i = 0; i < n; i++) {
		mint sum = 0;
		for (int j = 0; j < m; j++) {
			sum += x[j] * a[i][j];
		}
		if (a[i][m] != sum) {
			return 0;
		}
	}

	for (int i = 0; i < m; i++) {
		if (where[i] == -1) {
			return 2;
		}
	}
	return 1;
}