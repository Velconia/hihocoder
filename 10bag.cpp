#include <vector>
#include <iostream>

using std::vector;
using std::cout;
using std::cin;
using std::endl;

struct node {
    int value;
    int need;
    int value_earned;
    int need_left;

    explicit node(const int _n, const int _v) : 
        need(_n), value(_v), need_left(0), value_earned(0) { }
};

int main() {
    int count = 0;
    int points = 0;
    cin >> count >> points;
   
    // Initialization through the input 
    vector<node> v;
    while (count--) {
        int tmp_need = 0;
        int tmp_value = 0;
        
        cin >> tmp_need >> tmp_value;
        v.push_back(node(tmp_need, tmp_value));
    }

    int result = 0;
    // O(n^2)
    for (vector<node>::iterator i = v.begin(); i != v.end(); ++i) {
        for (vector<node>::iterator j = v.begin(); j != i; ++j) {
            if (j->need_left > i->need) {
                i->need_left = j->need_left - i
            }  
        }  
    }
}