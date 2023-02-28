#include <algorithm>
#include <cstdio>
#include <numeric>
#include <vector>

class DisjointSetUnion {
private:
	std::vector<int> par;
	std::vector<int> rank;
	int n;
public:
	// worst case O(n)
	DisjointSetUnion(int _n) {
		this->n = _n;
		rank.resize(n, 0);
		par.resize(n);
		std::iota(par.begin(), par.end(), 0);
	}
	// basically constant time
	int find(int x) {
		if(par[x] == x) // root
			return x;
		else // compress paths to children
			return par[x] = find(par[x]);
	}
	// basically constant time
	void sunion(int x, int y) {
		int px = find(x), py = find(y);
		if(px == py) return;
		// attach smaller child
		if(rank[px] < rank[py]) std::swap(px, py);
		if(rank[px] == rank[py]) rank[px]++;
		par[py] = px;
	}
};

int main() {
	DisjointSetUnion dsu(3);
	::printf("0: %d | 1: %d | 2: %d\n", dsu.find(0), dsu.find(1), dsu.find(2));
	dsu.sunion(0, 1);
	::printf("0: %d | 1: %d | 2: %d\n", dsu.find(0), dsu.find(1), dsu.find(2));
	dsu.sunion(0, 2);
	::printf("0: %d | 1: %d | 2: %d\n", dsu.find(0), dsu.find(1), dsu.find(2));
}
