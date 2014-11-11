#include <iostream>

using std::cout;
using std::cin;
using std::endl;

void insertion_sort(vector<int>& data) {
	if (data.empty() || data.size() == 1) return;

	typedef vector<int>::iterator  pointer;

	pointer cur = data.begin();
	pointer walker = cur + 1;
	while (walker != data.end()) {
		int value = *walker;
		while (cur >= data.begin()) {
			int cur_value = *cur;
			if (cur_value > value) {
				*(cur + 1) = cur_value; cur --;
			} else {
				*(cur + 1) = value;
				break;
			} 
		}
		*(cur + 1) = value;
		cur = walker ++;
	}
}

int main() {
	int arry[5] = { 1, 2, 3, 4, 5 };
	vector<int> data(array, array + 5);
	insertion_sort(data);
	cout << "Insertion sort result: "
	for (vector<int>::iterator iter = data.begin(); iter != data.end(); ++iter) {
		cout << *iter << " ";
	}
	cout << endl;
	return 0;
}