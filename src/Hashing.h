#pragma once
#include <bits/stdc++.h>
using namespace std;

// Arithmetic mod 2^64-1. 2x slower than mod 2^64 and more
// code, but works on evil test data (e.g. Thue-Morse, where
// ABBA... and BAAB... of length 2^10 hash the same mod 2^64).
// "typedef ull H;" instead if you think test data is random,
// or work mod 10^9+7 if the Birthday paradox is not a problem.
typedef uint64_t ull;
struct H {
	ull x;
	H(ull x = 0) : x(x) {}
	H operator+(H o) const { return x + o.x + (x + o.x < x); }
	H operator-(H o) const { return *this + ~o.x; }
	H operator*(H o) const {
		auto m = (__uint128_t)x * o.x;
		return H((ull)m) + (ull)(m >> 64);
	}
	ull get() const { return x + !~x; }
	bool operator==(H o) const { return get() == o.get(); }
	bool operator<(H o) const { return get() < o.get(); }
};
static const H C = (ull)1e11+3; // (order ~ 3e9; random also ok)

typedef unsigned short X;

vector<H> pw = {1};
void pw_resize(ull s) {
	while (pw.size() <= s) pw.push_back(pw.back() * C);
}

struct HashInterval {
	vector<H> ha;
	HashInterval(const vector<X>& str) : ha(str.size()+1) {
		pw_resize(str.size());
		for (ull j = 0; j < str.size(); j++) {
			ha[j+1] = ha[j] * C + str[j];
		}
	}
	H hashInterval(ull a, ull b) const { // hash [a, b)
		return ha[b] - ha[a] * pw[b - a];
	}
	void concat(const vector<X>& str) {
		ha.reserve(ha.size() + str.size());
		pw_resize(pw.size() + str.size());
		for (ull i = 0; i < str.size(); i++) {
			ha.push_back(ha.back() * C + str[i]);
		}
	}
};

H hashString(const vector<X>& s) {
	H h{};
	for(X c : s) h = h * C + c;
	return h;
}

// hashString(s + t) = concat(hashString(s), hashString(t), pw)
// Where pw is C**|t| and can be obtained from a HashInterval
H concat(H h0, H h1, H h1pw) {
	return h0 * h1pw + h1;
}

bool isSubStr(const HashInterval& s, const HashInterval& t) {
	ull n = s.ha.size() - 1, m = t.ha.size() - 1;
	if (n > m) {
		return false;
	}
	for (ull d = 0; d < m - n + 1; d++) {
		if (s.hashInterval(0, n) == t.hashInterval(d, d + n)) {
			return true;
		}
	}
	return false;
}
