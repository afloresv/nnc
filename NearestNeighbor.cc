#include <iostream>
#include <cstdio>
#include <vector>

using namespace std;

class NearestNeighbor : public KDtree {

	public:

	NearestNeighbor (Dataset &_D) : KDtree(_D) {}

	int of(int ind) {
		return search(ind);
	}

	bool classify(int ind) {
		return (D[search(ind)].c == D[ind].c);
	}
};
