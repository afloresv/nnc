#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <climits>
#include <algorithm>
#include <vector>
#include <set>

using namespace std;

class KDtree {

	protected:
	
	Dataset &D;
	int root, nn, sp;
	double rad;
	bool enemy;

	struct kdnode {
		int p,l,r;
		kdnode () {}
	} *T;

	struct kdorder {
		Dataset D;
		int k;
		kdorder(Dataset _D, int _k) : D(_D), k(_k) {}
		bool operator() (kdnode a, kdnode b) {
			return (D[a.p][k] < D[b.p][k]);
		}
	};

	int build (int l, int r, int k) {
		if (r-l<1) return -1;
		int m = (l+r)/2;
		kdorder ko(D,k);
		sort(T+l,T+r,ko);
		for (; m!=l && D[T[m].p][k]==D[T[m-1].p][k] ; m--);
		k = (k+1) % D.dim();
		T[m].l = build(l,m,k);
		T[m].r = build(m+1,r,k);
		return m;
	}

	void check (int ind) {
		if (enemy && D[ind].c==D[sp].c) return;
		if (sp==ind) {
			nn = ind;
			rad = -1.0;
			return;
		}
		double d = D.sqdist(ind,sp);
		if (d < rad) {
			nn = ind;
			rad = d;
		}
	}

	void search_rec (int ind, int k) {
		kdnode *n = &(T[ind]);
		if (n->l==-1 && n->r==-1) {
			check(n->p);
			return;
		}
		double d = D[n->p][k]-D[sp][k];
		k = (k+1) % D.dim();
		if (n->l==-1 || (n->r!=-1 && d<=0)) {
			search_rec(n->r,k);
			if (n->l!=-1 && rad > d*d)
				search_rec(n->l,k);
		} else {
			search_rec(n->l,k);
			if (n->r!=-1 && rad > d*d)
				search_rec(n->r,k);
		}
		check(n->p);
	}

	public:

	KDtree (Dataset &_D) : D(_D) {
		enemy = false;
	}

	void use (Subset &S) {
		int k=0;
		T = new kdnode[S.size()];
		for (int i=0 ; i<D.size() ; i++)
			if (S[i]) T[k++].p = i;
		root = build(0,k,0);
	}

	int search (int _sp) {
		sp = _sp;
		rad = numeric_limits<double>::max();
		search_rec(root,0);
		return nn;
	}
};
