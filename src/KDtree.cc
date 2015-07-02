#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <climits>
#include <algorithm>
#include <vector>
#include <set>
#include <queue>

using namespace std;

class KDtree {

	protected:
	
	Dataset &D;
	int root, spind, n;
	Point sp;
	bool smatch;
	double rad, sqrt_rad;
	priority_queue<pair<double,int> > nn;

	struct kdnode {
		int p,l,r,min,max;
		kdnode () {}
	};
	vector<kdnode> T;

	struct kdorder {
		Dataset &D;
		int k;
		kdorder(Dataset &_D, int _k) : D(_D), k(_k) {}
		bool operator() (kdnode a, kdnode b) {
			return (D[a.p][k] < D[b.p][k]);
		}
	};

	int build (int l, int r, int k) {
		if (r-l<1) return -1;
		int m = (l+r)/2;
		kdorder ko(D,k);
		//nth_element(T.begin()+l,T.begin()+m,T.begin()+r,ko);
		sort(T.begin()+l,T.begin()+r,ko);
		for (; m!=l && D[T[m].p][k]==D[T[m-1].p][k] ; m--);
		T[m].min = T[l].p;
		T[m].max = T[r-1].p;
		k = (k+1) % D.dim();
		T[m].l = build(l,m,k);
		T[m].r = build(m+1,r,k);
		return m;
	}

	void check (int ind) {
		double d = -1.0;
		if (spind==ind) {
			if (!smatch) return;
		} else d = D.sqdist(D[ind],sp);
		if (d<rad || (d==rad && D[ind].c==sp.c)) {
			nn.push(make_pair(d,ind));
			nn.pop();
			rad = nn.top().first;
			if (rad<0.0) sqrt_rad = 0.0;
			else sqrt_rad = sqrt(rad);
		}
	}

	void search_rec (int ind, int k) {
		kdnode *nd = &(T[ind]);
		if (nd->l==-1 && nd->r==-1) {
			check(nd->p);
			return;
		}
		double d = D[nd->p][k]-sp[k];
		int _k = (k+1) % D.dim();
		if (D[nd->p][k] <= sp[k]) {
			if (nd->r!=-1 && sp[k]-sqrt_rad<=D[nd->max][k])
				search_rec(nd->r,_k);
			if (sp[k]-sqrt_rad<=D[nd->p][k]) {
				check(nd->p);
				if (nd->l!=-1) search_rec(nd->l,_k);
			}
		} else {
			if (nd->l!=-1 && sp[k]+sqrt_rad>=D[nd->min][k])
				search_rec(nd->l,_k);
			if (sp[k]+sqrt_rad>=D[nd->p][k]) {
				check(nd->p);
				if (nd->r!=-1) search_rec(nd->r,_k);
			}
		}
	}

	vector<int> search_begin (int k) {
		while (!nn.empty()) nn.pop();
		rad = D.sqdist(D[T[root].p],sp);
		sqrt_rad = sqrt(rad);
		for (int i=0 ; i<k ; i++)
			nn.push(make_pair(rad,T[root].p));
		search_rec(root,0);
		vector<int> res(k);
		for (int i=k-1 ; i>=0 ; i--) {
			res[i] = nn.top().second;
			nn.pop();
		}
		return res;
	}

	public:

	KDtree (Dataset &_D) : D(_D) {
		smatch = true;
	}

	int size () { return n; }

	void use (Subset &S, int cls=-1) {
		n = 0;
		for (int i=0 ; i<D.size() ; i++)
			if (S[i] && cls!=D[i].c)
				n++;
		T = vector<kdnode>(n);
		n = 0;
		for (int i=0 ; i<D.size() ; i++)
			if (S[i] && cls!=D[i].c)
				T[n++].p = i;
		root = build(0,n,0);
	}

	void all (int cls=-1) {
		n = 0;
		for (int i=0 ; i<D.size() ; i++)
			if (cls!=D[i].c)
				n++;
		T = vector<kdnode>(n);
		n = 0;
		for (int i=0 ; i<D.size() ; i++)
			if (cls!=D[i].c)
				T[n++].p = i;
		root = build(0,n,0);
	}

	vector<int> search (Point _sp, int k) {
		sp = _sp;
		spind = -1;
		return search_begin(k);
	}

	int search (Point _sp) {
		return search(_sp,1)[0];
	}

	vector<int> search (int _spind, int k) {
		sp = D[_spind];
		spind = _spind;
		return search_begin(k);
	}

	int search (int _spind) {
		return search(_spind,1)[0];
	}
};
