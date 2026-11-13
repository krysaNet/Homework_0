module;

#include <vector>

export module my_stack;

export template<typename T>
class Stack {
    std::vector<T> data;

public:
    void push(T x) {
        data.push_back(std::move(x));
    }
    void pop() {
        data.pop_back();
    }
    auto& top() {
        return data.back();
    }
    auto size() const {
        return data.size();
    }
    auto empty() const {
        return data.empty();
    }
};