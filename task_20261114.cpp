#include <iostream>
import my_heap;
using namespace std;

int main() {
    Heap<int, heap_type::max_heap> h;
    {
        int n;
        cin >> n;
        while(n--) {
            int x;
            cin >> x;
            h.push(x);
        }
    }

    while(h.size() > 1) {
        auto y = h.pop();
        auto x = h.pop();
        if (y == x) continue;
        h.push(y - x);
    }

    if (h.empty()) cout << "0" << endl;
    else cout << h.top() << endl;

    return 0;
}