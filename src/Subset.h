#include <iostream>
#include <cstdio>
#include <vector>
#include "Dataset.h"

using namespace std;

#ifndef SUBSET_NNC_H
#define SUBSET_NNC_H

class Subset {
	int n;
	Dataset &D;
	vector<bool> is;

	public:
	Subset (Dataset &_D) : D(_D) {
		is = vector<bool>(D.size(),false);
		n = 0;
	}
	int size () {
		return n;
	}
	void set (int i, bool v=true) {
		if (is[i] && !v) n--;
		else if (!is[i] && v) n++;
		is[i] = v;
	}
	void flip (int i) {
		set(i,!is[i]);
	}
	bool operator[] (const int &i) {
		return is[i];
	}
	void operator=(const Subset &S) { 
		n = S.n;
		is = S.is;
	}
	void print () {
		fprintf(stderr, "Subset with %d points\n", n);
		for (int i=0,_n=0 ; i<is.size() ; i++)
			if (is[i]) printf((_n++==0 ? "%d" : ",%d"),D[i].ind);
		printf("\n");
	}
	static Subset intersect(Subset a, Subset b) {
		Subset s(a.D);
		for (int i=0 ; i<s.D.size() ; i++)
			s.set(i, a[i] && b[i]);
		return s;
	}
};

#endif
