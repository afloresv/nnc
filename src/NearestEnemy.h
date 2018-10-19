#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <utility>
#include "Dataset.h"
#include "Subset.h"
#include "KDtree.h"

using namespace std;

#ifndef NE_NNC_H
#define NE_NNC_H

class NearestEnemy {

	Dataset &D;
	vector<KDtree* > nn;
	NearestNeighbor *nnall;

	public:
	NearestEnemy (Dataset &_D) : D(_D) {
		nnall = new NearestNeighbor(D);
		for (int i=0 ; i<D.clss() ; i++)
			nn.push_back(new KDtree(D));
	}
	void use (Subset &S) {
		nnall->use(S);
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
	int of (Point p) {
		int ind = nnall->search(p);
		return nn[D[ind].c]->search(p);
	}
	double distance (int ind) {
		return D.distance(ind,of(ind));
	}
	double distance (Point p) {
		return D.distance(p,D[of(p)]);
	}
	double chrom_density (Point p) {
		double r1, rx;
		r1 = nnall->distance(p);
		rx = distance(p);
		return (rx-r1) / r1;
	}
	vector<pair<double,int> > order () {
		vector<pair<double,int> > ne;
		for (int i=D.size()-1 ; i>=0 ; i--)
			ne.push_back(make_pair(distance(i),i));
		sort(ne.begin(),ne.end());
		return ne;
	}
};

#endif
