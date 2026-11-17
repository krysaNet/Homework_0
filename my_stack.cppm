module;

#include <vector>
#include <stdexcept>

export module my_stack;

export template<typename T>
class Stack {
    std::vector<T> data;

public:
    void push(T x) {
        data.push_back(std::move(x));
    }

    void pop() {
        if (data.empty()) {
            throw std::out_of_range("Stack is empty: cannot call pop()");
        }
        data.pop_back();
    }

    auto& top() {
        if (data.empty()) {
            throw std::out_of_range("Stack is empty: cannot call top()");
        }
        return data.back();
    }

    const auto& top() const {
        if (data.empty()) {
            throw std::out_of_range("Stack is empty: cannot call top()");
        }
        return data.back();
    }

    auto size() const noexcept {
        return data.size();
    }

    auto empty() const noexcept {
        return data.empty();
    }
};