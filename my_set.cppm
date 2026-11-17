module;

#include <memory>
#include <stdexcept>

export module my_set;

enum class Position {root, left, right};

template<typename T>
struct node {
    std::unique_ptr<node> left;
    std::unique_ptr<node> right;
    node* parent;
    T data;

    explicit node(T x, node* p) : parent(p), data(std::move(x)) {}

    auto get_pos() const noexcept {
        if (parent == nullptr) return Position::root;
        return (this == parent->left.get()) ? Position::left : Position::right;
    }
};

export template<typename T> class Set {
    std::unique_ptr<node<T>> root;
    std::size_t len = 0uz;

    void del_node(node<T>* curr) noexcept {
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

public:
    auto size() const noexcept {
        return len;
    }
    auto empty() const noexcept {
        return !root;
    }

    void clear() noexcept {
        while (root) {
            if (root -> left) {
                auto left_child = std::move(root -> left);
                root -> left = std::move(left_child -> right);
                left_child -> right = std::move(root);
                root = std::move(left_child);
            }
            else {
                root = std::move(root -> right);
            }
        }
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
        const Set& host;

        friend class Set<T>;

        node<T>* get_child_for_erase() noexcept {
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

        void fall_left() noexcept {
            if (curr) {
                while (curr -> left) curr = curr->left.get();
            }
        }

    public:
        explicit iterator(node<T>* curr, const Set& s) noexcept : curr(curr), host(s) {}

        explicit iterator(const T& x, const Set& s, node<T>* root) : curr(root), host(s) {
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

        const auto& get_set() const noexcept {
            return host;
        }

        const auto& operator * () const {
            if (!curr) {
                throw std::out_of_range("Cannot get value at the end iterator");
            }
            return curr -> data;
        }

        bool operator == (const iterator& x) const noexcept {
            return curr == x.curr;
        }

        auto& operator ++ () {
            if (!curr) {
                throw std::out_of_range("Cannot increment the end iterator");
            }

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

    auto begin() const noexcept {
        iterator it{root.get(), *this};
        it.fall_left();
        return it;
    }

    auto end() const noexcept {
        return iterator{nullptr, *this};
    }

    void erase(iterator it) {
        if (&it.get_set() != this) {
            throw std::invalid_argument("Iterator belongs to another tree");
        }
        if (it == end()) {
            throw std::invalid_argument("Cannot erase end iterator");
        }

        node<T>* target = it.curr;
        if (auto ch = it.get_child_for_erase()) {
            target -> data = std::move(ch -> data);
            target = ch;
        }

        del_node(target);
        --len;
    }

    auto find(const T& x)const {
        return iterator{x, *this, root.get()};
    }

    void erase(const T& x) {
        auto it = find(x);
        if (it == end()) {
            throw std::out_of_range("Element not found");
        }
        erase(it);
    }

    bool contains(const T& x) const {
        return find(x) != end();
    }


    ~Set() {
        clear();
    }
};