#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <utility>

using namespace std;

class NearestEnemy {

	Dataset &D;
	vector<KDtree* > nn;

	public:
	NearestEnemy (Dataset &_D) : D(_D) {
		for (int i=0 ; i<D.clss() ; i++)
			nn.push_back(new KDtree(D));
	}
	void use (Subset &S) {
		for (int i=0 ; i<D.clss() ; i++)
			nn[i]->use(S,i);
	}
	void all () {
		for (int i=0 ; i<D.clss() ; i++)
			nn[i]->all(i);
	}
	int of (int ind) {
		return nn[D[ind].c]->search(ind);
	}
	double distance (int ind) {
		return D.distance(ind,of(ind));
	}
	vector<pair<double,int> > order () {
		vector<pair<double,int> > ne;
		for (int i=D.size()-1 ; i>=0 ; i--)
			ne.push_back(make_pair(distance(i),i));
		sort(ne.begin(),ne.end());
		return ne;
	}
};
