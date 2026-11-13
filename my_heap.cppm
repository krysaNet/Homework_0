module;

#include <iostream>
#include <string>
#include <memory>
#include <algorithm>

export module my_heap;
using namespace std;

export enum class heap_type {min_heap, max_heap};

export template <typename T, heap_type type>
class Heap {
    static constexpr bool compare(const T& parent, const T& child) {
        if constexpr (type == heap_type::max_heap) return child > parent;
        else return child < parent;
    }

    struct node : enable_shared_from_this<node> { // make_shared<...>(this) is safe now
        T data;
        shared_ptr<node> left = nullptr;
        shared_ptr<node> right = nullptr;
        weak_ptr<node> parent;

        node() = default;
        explicit node(T x) : data(x) {}
        explicit node(T x, const shared_ptr<node>& p) : data(x), parent(p) {}

        void sift_up() {
            auto p = parent.lock();
            if (!p) return;

            if (compare(p -> data, data)) {
                swap(p -> data, data);
                p -> sift_up();
            }
        }

        void sift_down() {
            shared_ptr<node> target = this -> shared_from_this();

            if (left && compare(target -> data, left -> data)) {
                target = left;
            }
            if (right && compare(target -> data, right -> data)) {
                target = right;
            }

            if (target.get() != this) {
                swap(target -> data, data);
                target -> sift_down();
            }
        }
    };

    shared_ptr<node> root = nullptr;
    size_t len = 0;

    auto find_last_parent(const size_t index) const {
        if (index <= 1) return pair<shared_ptr<node>, bool>{};

        string route;
        size_t temp = index;
        while (temp > 1) {
            route += (temp % 2 == 0) ? '0' : '1';
            temp /= 2;
        }
        reverse(route.begin(), route.end());

        auto curr = root;
        for (size_t i = 0; i < route.length() - 1; ++i) {
            if (route[i] == '0') curr = curr -> left;
            else curr = curr -> right;
        }

        bool is_right = (route.back() == '1');
        return make_pair(curr, is_right);
    }

public:
    [[nodiscard]] size_t size() const {
        return len;
    }
    [[nodiscard]] bool empty() const {
        return len == 0;
    }

    T top() const {
        if (empty()) {
            cout << "Heap is empty" << endl;
            exit(1);
        }
        return root -> data;
    }

    void push(T x) {
        ++len;
        if (!root) {
            root = make_shared<node>(x);
        }
        else {
            auto [parent_node, is_right] = find_last_parent(len);
            auto new_node = make_shared<node>(x, parent_node);

            if (is_right) parent_node -> right = new_node;
            else parent_node -> left = new_node;

            new_node -> sift_up();
        }
    }

    T pop() {
        T x = top();
        if (len == 1) {
            root = nullptr;
            len = 0;
            return x;
        }

        auto [last_parent_node, is_right] = find_last_parent(len);
        shared_ptr<node> last_node = is_right ? last_parent_node -> right : last_parent_node -> left;

        root -> data = move(last_node -> data);

        if (is_right) last_parent_node -> right = nullptr;
        else last_parent_node -> left = nullptr;

        --len;

        root -> sift_down();

        return x;
    }
};