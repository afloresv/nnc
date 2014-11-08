#include <iostream>
#include <cstdio>
#include <vector>

using namespace std;

class Subset {
	int n;
	vector<bool> is;

	public:
	Subset() {}
	Subset(Dataset D) {
		is = vector<bool>(D.size());
		n = 0;
	}
	int size() {
		return n;
	}
	void set(int i, bool v=true) {
		if (is[i] && !v) n--;
		else if (!is[i] && v) n++;
		is[i] = v;
	}
	void flip(int i) {
		set(i,!is[i]);
	}
	bool operator[](const int& i) {
		return is[i];
	}
};
