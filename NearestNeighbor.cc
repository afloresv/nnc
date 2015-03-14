#include <iostream>
#include <cstdio>
#include <algorithm>
#include <set>
#include <vector>

using namespace std;

class NearestNeighbor : public KDtree {

	char vote (vector<int> nn) {
		multiset<char> ms;
		int mx = 0, k = nn.size();
		for (int i=0 ; i<k ; i++) {
			ms.insert(D[nn[i]].c);
			mx = max(mx,(int)ms.count(D[nn[i]].c));
		}
		for (int i=0 ; i<k ; i++)
			if (ms.count(D[nn[i]].c) == mx)
				return D[nn[i]].c;
		return 0;
	}

	public:
	NearestNeighbor (Dataset &_D) : KDtree(_D) {}
	int of (int ind) {
		return search(ind);
	}
	int of (Point p) {
		return search(p);
	}
	bool classify (int ind, int k=1) {
		if (k==1) return (D[search(ind)].c == D[ind].c);
		return (vote(search(ind,k)) == D[ind].c);
	}
	bool classify (Point p, int k=1) {
		if (k==1) return (D[search(p)].c == p.c);
		return (vote(search(p,k)) == p.c);
	}
	void selfmatch (bool sm) {
		smatch = sm;
	}
	int wrong (int k=1) {
		int w=0;
		for (int i=D.size()-1 ; i>=0 ; i--)
			if (!classify(i,k)) w++;
		return w;
	}
	int wrong (Dataset &T, int k=1) {
		int w=0;
		for (int i=T.size()-1 ; i>=0 ; i--)
			if (!classify(T[i],k)) w++;
		return w;
	}
};
