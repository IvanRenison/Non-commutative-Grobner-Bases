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

struct HashInterval {
	vector<H> ha, pw;
	HashInterval(const vector<X>& str) : ha(str.size()+1), pw(ha) {
		pw[0] = 1;
		for (ull i = 0; i < str.size(); i++) {
			ha[i+1] = ha[i] * C + str[i],
			pw[i+1] = pw[i] * C;
		}
	}
	H hashInterval(ull a, ull b) const { // hash [a, b)
		return ha[b] - ha[a] * pw[b - a];
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
