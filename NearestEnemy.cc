#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <utility>

using namespace std;

class NearestEnemy {

	Dataset &D;
	vector<KDtree> nn;

	public:
	NearestEnemy (Dataset &_D) : D(_D) {
		for (int i=0 ; i<D.clss() ; i++)
			nn.push_back(KDtree(D));
	}
	void use (Subset &S) {
		for (int i=0 ; i<D.clss() ; i++)
			nn[i].use(S,i);
	}
	void all () {
		for (int i=0 ; i<D.clss() ; i++)
			nn[i].all(i);
	}
	int of (int ind) {
		int ne = -1;
		double dne = numeric_limits<double>::max();
		for (int i=0 ; i<D.clss() ; i++) {
			if (D[ind].c==i || nn[i].size()==0) continue;
			int n = nn[i].search(ind);
			double dn = D.distance(ind,n);
			if (dn < dne) {
				ne = n;
				dne = dn;
			}
		}
		return ne;
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
