#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <utility>

using namespace std;

class NearestEnemy : public KDtree {

	public:
	NearestEnemy (Dataset &_D) : KDtree(_D) {
		enemy = true;
	}
	int of (int ind) {
		return search(ind);
	}
	double distance (int ind) {
		return D.distance(ind,search(ind));
	}
	vector<pair<double,int> > order () {
		vector<pair<double,int> > ne;
		for (int i=D.size()-1 ; i>=0 ; i--)
			ne.push_back(make_pair(distance(i),i));
		sort(ne.begin(),ne.end());
		return ne;
	}
};
