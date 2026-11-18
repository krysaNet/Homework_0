#include <iostream>
import my_stack;
using namespace std;

template<typename T>
void sort_stack(Stack<T> &x) {
    Stack<T> y;
    while (!x.empty()) {
        auto tp = x.top();
        x.pop();

        while (!y.empty() && tp > y.top()) {
            x.push(y.top());
            y.pop();
        }
        y.push(tp);
    }
    x = std::move(y);
}

int main() {
    Stack<int> x;
    x.push(1);
    x.push(2);
    x.push(52);
    x.push(3);
    x.push(4);
    sort_stack(x);
    while (!x.empty()) {
        cout << x.top() << endl;
        x.pop();
    }
    return 0;
}