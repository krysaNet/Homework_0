module;

#include <memory>

export module my_set;

enum class Position {root, left, right};

template<typename T>
struct node {
    std::unique_ptr<node> left;
    std::unique_ptr<node> right;
    node* parent;
    T data;

    explicit node(T x, node* p) : parent(p), data(std::move(x)) {}

    auto get_pos() const {
        if (parent == nullptr) return Position::root;
        return (this == parent->left.get()) ? Position::left : Position::right;
    }
};

export template<typename T> class Set {
    std::unique_ptr<node<T>> root;
    std::size_t len = 0uz;

public:
    auto size() const {
        return len;
    }
    auto empty() const {
        return !root;
    }
    void clear() {
        root = nullptr;
        len = 0uz;
    }

    void insert(T x) {
        if (!root) {
            root = std::make_unique<node<T>>(std::move(x), nullptr);
            ++len;
            return;
        }
        node<T>* curr = root.get();
        while (true) {
            if (curr -> data == x) return;
            if (x < curr -> data) {
                if (curr -> left) curr = curr->left.get();
                else {
                    curr -> left = std::make_unique<node<T>>(std::move(x), curr);
                    ++len;
                    return;
                }
            }
            else {
                if (curr -> right) curr = curr->right.get();
                else {
                    curr -> right = std::make_unique<node<T>>(std::move(x), curr);
                    ++len;
                    return;
                }
            }
        }
    }

    class iterator {
        node<T>* curr;

        friend class Set<T>;

        node<T>* get_child_for_erase() {
            if (!curr) return nullptr;
            auto res = curr;
            if (res -> right) {
                res = res->right.get();
                while (true) {
                    if (res -> left) res = res->left.get();
                    else break;
                }
                return res;
            }
            if (res -> left) {
                res = res->left.get();
                while (true) {
                    if (res -> right) res = res->right.get();
                    else break;
                }
                return res;
            }
            return nullptr;
        }

        void fall_left() {
            if (curr) {
                while (curr -> left) curr = curr->left.get();
            }
        }

    public:
        explicit iterator(node<T>* curr) : curr(curr) {}

        explicit iterator(const T& x, node<T>* root) : curr(root) {
            while (curr != nullptr) {
                if (curr -> data == x) break;
                if (x < curr -> data) {
                    curr = curr->left.get();
                }
                else {
                    curr = curr->right.get();
                }
            }
        }

        const auto& operator * () const {
            return curr -> data;
        }

        bool operator == (const iterator& x) const = default;

        auto& operator ++ () {
            if (!curr) return *this;

            if (curr -> right) {
                curr = curr->right.get();
                fall_left();
            }
            else {
                while (curr -> get_pos() == Position::right) {
                    curr = curr -> parent;
                }
                curr = curr -> parent;
            }

            return *this;
        }
    };

    auto begin() const {
        iterator it{root.get()};
        it.fall_left();
        return it;
    }

    auto end() const {
        return iterator{nullptr};
    }

    bool contains(const T& x) const {
        iterator it{x, root.get()};
        return it.curr != nullptr;
    }

    void del_node(node<T>* curr) {
        auto child = curr->right ? std::move(curr->right) : std::move(curr->left);

        if (child) {
            child -> parent = curr -> parent;
        }

        auto p = curr -> get_pos();
        if (p == Position::root) {
            root = std::move(child);
        }
        else if (p == Position::left) {
            curr -> parent -> left = std::move(child);
        }
        else {
            curr -> parent -> right = std::move(child);
        }
    }
    void erase(iterator it) {
        if (it == end()) return;

        node<T>* target = it.curr;
        if (auto ch = it.get_child_for_erase()) {
            target -> data = std::move(ch -> data);
            target = ch;
        }

        del_node(target);
        --len;
    }
};