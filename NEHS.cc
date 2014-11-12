#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>

using namespace std;

class NEHS {

	Dataset &D;
	int l, r;
	vector<pair<double,int> > neo;

	public:

	NEHS (Dataset &_D) : D(_D) {
		NearestEnemy NE(D);
		NE.all();
		neo = NE.order();
		l = 0;
		r = D.size()-1;
	}

	void percentile (double _l, double _r) {
		l = min(D.size()-1, max(0, (int) (D.size() * _l)));
		r = min(D.size()-1, max(0, (int) (D.size() * _r)));
	}

	void band (double m, double ep) {
		pair<double,int> s;
		s = make_pair(m-ep,0);
		l = neo.begin() - lower_bound(neo.begin(),neo.end(),s);
		l = min(D.size()-1,max(0,l));
		s = make_pair(m+ep,D.size());
		r = neo.begin() - lower_bound(neo.begin(),neo.end(),s);
		r = min(D.size()-1,max(0,r));
	}

	Subset run () {
		Subset S(D);
		vector<int> sel;
		bool add;
		for (int i=r ; i>=l ; i--) {
			add = true;
			for (int j=sel.size()-1 ; add && j>=0 ; j--)
				if (D.distance(neo[i].second,neo[sel[j]].second)
					< neo[i].first + neo[sel[j]].first)
					add = false;
			if (add) {
				sel.push_back(i);
				S.set(neo[i].second);
			}
		}
		return S;
	}
};
