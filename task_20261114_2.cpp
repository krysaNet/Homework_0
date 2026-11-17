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
    x = y;
}

int main() {
    return 0;
}