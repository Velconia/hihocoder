#include <iostream>
#include <vector>

using std::cout;
using std::cin;
using std::endl;
using std::vector;

int main() {
	int row_count = 0;
	int row_index = 0;
	vector<int> best;
	vector<int> cur;
	vector<int> tmp;

	cin >> row_count;

	while (row_count--) {
		int index = ++row_index;

		if (index == 1) {
			int tmp = 0;
			cin >> tmp;
			best.push_back(tmp);
		} else {
			while (index--) {
				int tmp = 0; cin >> tmp;
				cur.push_back(tmp);
				tmp.push_back(-1);
			} 

			for (vector<int>::iterator i = best.begin(), cIndex = cur.begin(), tmpIndex = tmp.begin();
				 i != best.end(); ++i) {
				const int left = *cIndex++ + *i; *tmpIndex = *tmpIndex >= left ? *tmpIndex : left; tmpIndex++;
				const int right = *cIndex + *i; *tmpIndex = *tmpIndex >= right ? *tmpIndex : right;	
			}

			best = tmp; cur.clear(); tmp.clear();
		}
	}

	int result = -1;
	for (vector<int>::iterator i = best.begin(); i != best.end(); ++i) {
		result = result >= *i ? result : *i;
	}	
	cout << result << endl;
}