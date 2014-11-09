#include <iostream>
#include <cstdio>
#include <vector>

using namespace std;

class NearestEnemy : public KDtree {

	public:

	NearestEnemy (Dataset &_D) : KDtree(_D) {
		enemy = true;
	}

	int of(int ind) {
		return search(ind);
	}

	double distance(int ind) {
		return D.distance(search(ind));
	}
};
