#include <iostream>
#include <cstdio>
#include <vector>
#include "Dataset.h"
#include "Subset.h"
#include "KDtree.h"
#include "NearestNeighbor.h"
#include "NearestEnemy.h"
#include "IO.h"

using namespace std;

#ifndef ALG_NNC_H
#define ALG_NNC_H


// +---------------------+
// |    CNN Algorithm    |
// +---------------------+

Subset CNN (Dataset &D) {
	Subset R(D);
	NearestNeighbor NN(D);
	R.set(0);
	NN.use(R);
	while (true) {
		for (int i=0 ; i<D.size() ; i++) {
			if (!NN.classify(i)) {
				R.set(i);
				NN.use(R);
			}
		}
		if (NN.wrong()==0) break;
	}
	return R;
}


// +---------------------+
// |    RNN Algorithm    |
// +---------------------+

Subset RNN (Dataset &D) {
	Subset R(D);
	NearestNeighbor NN(D);
	R = CNN(D);
	NN.use(R);
	for (int i=0 ; i<D.size() ; i++) {
		if (R[i]) {
			R.set(i,false);
			NN.use(R);
			if (NN.wrong()!=0) R.set(i);
		}
	}
	return R;
}


// +----------------------+
// | Alpha-FCNN Algorithm |
// +----------------------+

Subset AlphaFCNN (Dataset &D, double alpha) {
	Subset R(D);
	vector<int> S = D.centroids(),
		nn(D.size(),-1),
		ne(D.size(),-1),
		rep(D.size(),-1);
	vector<double> dnn(D.size(),numeric_limits<double>::max()),
	               dne(D.size(),numeric_limits<double>::max());

	while (S.size() > 0) {
		for (int i=0 ; i<S.size() ; i++) R.set(S[i]);
		for (int i=0 ; i<D.size() ; i++)
			if (R[i]) rep[i] = -1;
		for (int i=0 ; i<D.size() ; i++) {
			if (R[i]) continue;
			for (int j=0 ; j<S.size() ; j++) {
				double d = D.distance(i,S[j]);
				if (d < dnn[i]) {
					nn[i] = S[j];
					dnn[i] = d;
				}
				if (D[i].c != D[S[j]].c && d < dne[i]) {
					ne[i] = S[j];
					dne[i] = d;
				}
			}
			if (D[i].c != D[nn[i]].c && (rep[nn[i]]==-1
				|| dnn[i] < D.distance(nn[i],rep[nn[i]])))
				rep[nn[i]] = i;
		}
		S.clear();
		for (int i=0 ; i<D.size() ; i++)
			if (R[i] && rep[i]!=-1)
				S.push_back(rep[i]);
	}
	return R;
}


// +----------------------+
// |    FCNN Algorithm    |
// +----------------------+

Subset FCNN (Dataset &D) {
	return AlphaFCNN(D, 0.0f);
}


// +---------------------+
// |    NET Algorithm    |
// +---------------------+

Subset NET (Dataset &D) {
	Subset R(D);
	NearestNeighbor NN(D);
	R.set(0);
	NN.use(R);
	NearestEnemy NE(D);
	NE.all();
	vector<pair<double,int> > neo = NE.order();
	double gamma = neo[0].first;
	bool add;

	// Original NET - O(n^2)
	for (int i=1 ; i<D.size() ; i++) {
		add = true;
		for (int j=0 ; j<D.size() ; j++) {
			if (!R[j]) continue;
			if (D.distance(i,j) < gamma) {
				add = false;
				break;
			}
		}
		if (add) R.set(i);
	}

	// + Prunning Heuristic
	for (int i=D.size()-1 ; i>=0 ; i--) {
		if (!R[neo[i].second]) continue;
		for (int j=i-1 ; j>=0 ; j--) {
			if (!R[neo[j].second]) continue;
			if (D.distance(neo[i].second,neo[j].second) < neo[i].first / 2.0f - gamma)
				R.flip(neo[j].second);
		}
	}

	return R;
}

// +---------------------+
// |    MSS Algorithm    |
// +---------------------+

Subset AlphaMSS (Dataset &D, double alpha) {
	NearestEnemy NE(D);
	NE.all();
	vector<pair<double,int> > neo = NE.order();
	Subset R(D), S(D,true);
	bool add;
	for (int i=0 ; i<D.size() ; i++) {
		add = false;
		for (int j=i ; j<D.size() ; j++) {
			if (S[j] && D[neo[i].second].c == D[neo[j].second].c &&
				D.distance(neo[i].second,neo[j].second) * (1.0f + alpha)
					< neo[j].first) {
				add = true;
				S.set(j,false);
			}
		}
		if (add) R.set(neo[i].second);
	}
	return R;
}

Subset MSS (Dataset &D) {
	return AlphaMSS(D, 0.0f);
}

// +---------------------+
// |    RSS Algorithm    |
// +---------------------+

Subset AlphaRSS (Dataset &D, double alpha) {
	NearestEnemy NE(D);
	NE.all();
	vector<pair<double,int> > neo = NE.order();
	Subset R(D);
	vector<int> sel;
	bool add;
	for (int i=0 ; i<D.size() ; i++) {
		add = true;
		for (int j=sel.size()-1 ; add && j>=0 ; j--)
			if (D[neo[i].second].c == D[sel[j]].c &&
				D.distance(neo[i].second,sel[j]) * (1.0f + alpha)
					< neo[i].first)
				add = false;
		if (add) {
			sel.push_back(neo[i].second);
			R.set(neo[i].second);
		}
	}
	return R;
}

Subset RSS (Dataset &D) {
	return AlphaRSS(D, 0.0f);
}

// +---------------------+
// |    VSS Algorithm    |
// +---------------------+

Subset VSS (Dataset &D) {
	NearestEnemy NE(D);
	NE.all();
	vector<pair<double,int> > neo = NE.order();
	Subset R(D);
	vector<int> sel;
	bool add;
	int s_ind, t_ind;
	double s_dst, ne_dst, tmp;
	for (int i=0 ; i<D.size() ; i++) {
		add = true;
		for (int j=sel.size()-1 ; add && j>=0 ; j--)
			if (D[neo[i].second].c == D[sel[j]].c &&
				D.distance(neo[i].second,sel[j])
					< neo[i].first)
				add = false;
		if (add) {
			t_ind = neo[i].second;
			s_ind = t_ind;
			ne_dst = NE.distance(t_ind);
			s_dst = ne_dst / 2.0;
			for (int j=0 ; j<D.size() ; j++) {
				if (D[t_ind].c == D[j].c && D.distance(t_ind,j) < ne_dst) {
					tmp = D.pivotdist(t_ind, NE.of(t_ind), j);
					if (tmp < s_dst) {
						s_dst = tmp;
						s_ind = j;
					}
				}
			}
			R.set(s_ind);
			sel.push_back(s_ind);
		}
	}
	return R;
}

// +---------------------+
// |    HSS Algorithm    |
// +---------------------+

Subset AlphaHSS (Dataset &D, double alpha) {
	NearestEnemy NE(D);
	NE.all();
	vector<int> num_friends(D.size(),1);
	vector<double> dne(D.size());
	for (int i=D.size()-1 ; i>=0 ; i--) {
		dne[i] = NE.distance(i);
		for (int j=D.size()-1 ; j>=0 ; j--)
			if (i!=j && D[i].c==D[j].c && D.distance(i,j) < dne[i]/(1.0f+alpha))
				num_friends[j]++;
	}
	Subset R(D), C(D);
	while (C.size() < D.size()) {
		int idx = -1;
		for (int i=D.size()-1 ; i>=0 ; i--) {
			if (R[i]) continue;
			if (idx==-1 || num_friends[idx] < num_friends[i])
				idx = i;
		}
		R.set(idx);
		for (int i=D.size()-1 ; i>=0 ; i--) {
			if (C[i]) continue;
			if (i==idx || (D[i].c==D[idx].c && D.distance(i,idx) < dne[i]/(1.0f+alpha))) {
				C.set(i);
				for (int j=D.size()-1 ; j>=0 ; j--)
					if (i==j || (D[i].c==D[j].c && D.distance(i,j) < dne[i]/(1.0f+alpha)))
						num_friends[j]--;
			}
		}
	}
	return R;
}

Subset HSS (Dataset &D) {
	return AlphaHSS(D, 0.0f);
}


#endif
