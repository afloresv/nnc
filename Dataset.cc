#include <iostream>
#include <cstdio>
#include <cmath>

using namespace std;

class Point {
	double *p;
	public:
	char c;
	Point () {}
	Point (int m) {
		p = new double[m];
	}
	double& operator[](const int& i) {
		return p[i];
	}
};

class Dataset {
	int n, m;
	Point *p;

	public:
	Dataset() {}
	Dataset(int _n, int _m) : n(_n), m(_m) {
		p = new Point[n];
		for (int i=0 ; i<n ; i++)
			p[i] = Point(m);
	}
	int size() { return n; }
	int dim()  { return m; }
	Point& operator[](const int& i) {
		return p[i];
	}
	double distance(int a, int b) {
		return sqrt(sqdist(a,b));
	}
	double sqdist(int a, int b) {
		double dist = 0.0;
		for (int i=0 ; i<m ; i++)
			dist += pow(p[a][i] - p[b][i],2);
		return dist;
	}
};
