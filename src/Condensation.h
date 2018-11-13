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

	for (int i=1 ; i<D.size() ; i++) {
		if (i % 100 == 0) cout << i << endl;
		if (NN.distance(i) >= gamma) {
			R.set(i);
			NN.use(R);
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
			if (D[neo[i].second].c == D[neo[sel[j]].second].c &&
				D.distance(neo[i].second,neo[sel[j]].second) * (1.0f + alpha)
					< neo[i].first)
				add = false;
		if (add) {
			sel.push_back(i);
			R.set(neo[i].second);
		}
	}
	return R;
}

Subset RSS (Dataset &D) {
	return AlphaRSS(D, 0.0f);
}

#endif
