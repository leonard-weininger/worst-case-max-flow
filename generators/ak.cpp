/* generator of hard maxflow problems */
/* 01/09/94 - Stanford Computer Science Department */
/* Boris Cherkassky - cher@theory.stanford.edu, on.cher@zib-berlin.de */
/* Andrew V. Goldberg - goldberg@cs.stanford.edu */

/*
 * Modified to have the following input/output format:
 *   Input: 2 integers from stdin: max_n, max_m
 *   Output: Graph to stdout in the format:
 *     n m
 *     from_1 to_1 cap_1
 *     ...
 *     from_m to_m cap_m
 *   Vertices are numbered from 1 to n, 1 = source, n = sink.
 */

#include <cassert>
#include <iostream>

int main () {
	int max_n, max_m;
	std::cin >> max_n >> max_m;

	assert(max_n >= 14);
	assert(max_m >= 19);

	int n = 2;

	while (4 * (n + 1) + 6 <= max_n && 6 * (n + 1) + 7 <= max_m) {
		++n;
	}

	std::cout << 4 * n + 6 << ' ' << 6 * n + 7 << '\n';

	/* first terrible graph */

	for (int i = 0; i < n ; ++i) {
		std::cout << i + 2 << ' ' << i + 3 << ' ' << n - i + 1 << '\n';
		std::cout << i + 2 << ' ' << n + 3 << ' ' << 1 << '\n';
	}

	std::cout << n + 2 << ' ' << 2 * n + 3 << ' ' << 1 << '\n';
	std::cout << n + 2 << ' ' << n + 3 << ' ' << 1 << '\n';

	for (int i = n + 3; i <= 2 * n + 2; ++i) {
		std::cout << i << ' ' << i + 1 << ' ' << n + 1 << '\n';
	}

	/* second horrible graph */

	int d = 2 * n + 4;

	for (int i = d; i <= 2 * n + d; ++i) {
		std::cout << i << ' ' << i + 1 << ' ' << n << '\n';
	}

	for (int i = 0; i < n; ++i) {
		std::cout << i + d << ' ' << 2 * n + 1 - i + d << ' ' << 1 << '\n';
	}

	/* edges from source and to sink */

	std::cout << "1 2 1000000000" << '\n';
	std::cout << "1 " << d << " 1000000000" << '\n';
	std::cout << d - 1 << ' ' << 4 * n + 6 << " 1000000000" << '\n';
	std::cout << 4 * n + 5 << ' ' << 4 * n + 6 << " 1000000000" << '\n';

	return 0;
}

