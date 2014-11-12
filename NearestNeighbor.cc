#include <iostream>
#include <cstdio>
#include <vector>

using namespace std;

class NearestNeighbor : public KDtree {
	public:
	NearestNeighbor (Dataset &_D) : KDtree(_D) {}
	int of (int ind) {
		return search(ind);
	}
	bool classify (int ind) {
		return (D[search(ind)].c == D[ind].c);
	}
	void selfmatch (bool sm) {
		smatch = sm;
	}
	int wrong () {
		int w=0;
		for (int i=D.size()-1 ; i>=0 ; i--)
			if (!classify(i)) w++;
		return w;
	}
};
