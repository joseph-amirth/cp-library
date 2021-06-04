#include "Point.h"
#include <vector>
#include <assert.h>
#include <numeric>
#include <algorithm>

template <typename T>
std::pair<int, int> closest_pair_of_points(const std::vector<point<T>> &p) {
	int n = int(p.size());
	assert(n > 1);

	std::vector<int> ind(n), temp(n);
	std::iota(ind.begin(), ind.end(), 0);
	std::sort(ind.begin(), ind.end(), [&](int i, int j) {
		return p[i] < p[j];
	});

	std::pair<int, int> best_pair(0, 1);
	auto best_distance = dist(p[0], p[1]);

	auto recurse = [&](int l, int r, const auto &self) -> void {
		if (r - l < 3) {
			for (int i = l; i < r; i++) {
				for (int j = i + 1; j <= r; j++) {
					if (dist(p[ind[i]], p[ind[j]]) < best_distance) {
						best_pair = std::make_pair(ind[i], ind[j]);
						best_distance = dist(p[ind[i]], p[ind[j]]);
					}
				}
			}
			std::sort(ind.begin() + l, ind.begin() + r + 1, [&](int i, int j) {
				return p[i].y < p[j].y;
			});
			return;
		}

		int mid = (l + r) >> 1;
		T mid_x = p[ind[mid]].x;
		self(l, mid, self);
		self(mid + 1, r, self);

		std::merge(ind.begin() + l, ind.begin() + mid + 1, ind.begin() + mid + 1, ind.begin() + r + 1, temp.begin(), [&](int i, int j) {
			return p[i].y < p[j].y;
		});

		std::copy(temp.begin(), temp.begin() + r - l + 1, ind.begin() + l);

		for (int i = l, k = 0; i <= r; i++) {
			if (std::abs(p[ind[i]].x - mid_x) < best_distance) {
				for (int j = k - 1; j >= 0 && p[ind[i]].y - p[temp[j]].y < best_distance; j--) {
					if (dist(p[ind[i]], p[temp[j]]) < best_distance) {
						best_pair = std::make_pair(ind[i], temp[j]);
						best_distance = dist(p[ind[i]], p[temp[j]]);
					}
				}
				temp[k++] = ind[i];
			}
		}
	};

	recurse(0, n - 1, recurse);
	return best_pair;
}