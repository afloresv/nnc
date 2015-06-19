#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <string>
#include <map>
#include <algorithm>
#include <set>
#include <utility>
#include <limits>

using namespace std;

class Point {
	double *p;
	public:
	int c, ind;
	Point () {}
	Point (int m) {
		p = new double[m];
	}
	double& operator[](const int& i) {
		return p[i];
	}
};

unsigned long long CountDistances=0ULL;

class Dataset {
	int n, m, c;
	Point *p;

	public:
	Dataset () {}
	Dataset (int _n, int _m, int _c) : n(_n), m(_m), c(_c) {
		p = new Point[n];
		for (int i=0 ; i<n ; i++)
			p[i] = Point(m);
	}
	int size () { return n; }
	int dim  () { return m; }
	int clss () { return c; }
	Point& operator[] (const int& i) {
		return p[i];
	}
	double distance (int a, int b) {
		return sqrt(sqdistance(a,b));
	}
	double distance (Point a, Point b) {
		return sqrt(sqdist(a,b));
	}
	double sqdistance (int a, int b) {
		return sqdist(p[a],p[b]);
	}
	double sqdist (Point &a, Point &b) {
		CountDistances++;
		double dist = 0.0;
		for (int i=0 ; i<m ; i++)
			dist += pow(a[i]-b[i], 2);
		return dist;
	}
	void shuffle () {
		for (int i=0 ; i<n ; i++)
			swap(p[i],p[i + (rand() % (n-i))]);
	}
	vector<int> centroids () {
		Point *avg = new Point[c];
		int *num = new int[c];
		for (int i=0 ; i<c ; i++) {
			avg[i] = Point(m);
			for (int j=m-1 ; j>=0 ; j--)
				avg[i][j] = 0.0;
			num[i] = 0;
		}
		for (int i=0 ; i<n ; i++) {
			for (int j=m-1 ; j>=0 ; j--)
				avg[p[i].c][j] += p[i][j];
			num[p[i].c]++;
		}
		for (int i=0 ; i<c ; i++)
			for (int j=0 ; j<m ; j++)
				avg[i][j] /= (double)num[i];
		vector<int> res(c);
		vector<double> d(c,numeric_limits<double>::max());
		for (int i=0 ; i<n ; i++) {
			double r = distance(p[i],avg[p[i].c]);
			if (r < d[p[i].c]) {
				d[p[i].c] = r;
				res[p[i].c] = i;
			}
		}
		return res;
	}
};
