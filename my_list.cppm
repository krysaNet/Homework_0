module;

#include <iostream>
#include <memory>

export module my_list;

export template<typename T>
class List {
    template<typename T1>
    struct Node {
        T1 data;
        std::unique_ptr<Node<T1>> next;
        Node<T1>* prev;

        Node(auto val, std::unique_ptr<Node<T1>> next = nullptr, Node<T1>* prev = nullptr) :
            data(std::move(val)), next(std::move(next)), prev(prev) {}
    };

    std::unique_ptr<Node<T>> head = nullptr;
    Node<T>* tail = nullptr;
    std::size_t len = 0uz;

public:

    void push_back(T val) {
        auto x = std::make_unique<Node<T>>(std::move(val), nullptr, tail);
        Node<T>* raw = x.get();
        if (tail) {
            tail -> next = std::move(x);
        } else {
            head = std::move(x);
        }
        tail = raw;
        ++len;
    }

    void push_front(T val) {
        auto x = std::make_unique<Node<T>>(std::move(val), std::move(head), nullptr);
        if (head) {
            head -> prev = x.get();
        } else {
            tail = x.get();
        }
        head = std::move(x);
        ++len;
    }

    void clear() {
        len = 0uz;
        head = nullptr;
        tail = nullptr;
    }

    void pop_back() {
        if (tail == nullptr) {
            std::cout << "List is empty" << std::endl;
            exit(1);
        }
        if (tail -> prev == nullptr) {
            clear();
            return;
        }
        tail = tail -> prev;
        tail -> next = nullptr;
        --len;
    }

    void pop_front() {
        if (!head) {
            std::cout << "List is empty" << std::endl;
            exit(1);
        }
        if (!head -> next) {
            clear();
            return;
        }
        head = std::move(head -> next);
        head -> prev = nullptr;
        --len;
    }

    class iterator {
        Node<T>* current;
        List<T>& list;

    public:
        explicit iterator(List<T>& l) : current(nullptr), list(l) {}
        explicit iterator(Node<T>* curr, List<T>& l) : current(curr), list(l) {}

        auto operator == (const iterator& it) const {
            return current == it.current;
        }

        auto operator != (const iterator& it) const {
            return current != it.current;
        }

        auto erase() {
            if (current -> prev == nullptr) list.pop_front();
            else if (current->next.get() == nullptr) list.pop_back();
            else {
                current -> next -> prev = current -> prev;
                current -> prev -> next = std::move(current -> next);
            }
            current = nullptr;
        }

        auto& operator * () {
            if (!current) {
                std::cout << "Iterator is empty" << std::endl;
                exit(1);
            }
            return current -> data;
        }

        auto operator ++ () {
            current = current -> next.get();
            return *this;
        }

        auto operator -- () {
            current = current -> prev;
            return *this;
        }

        void push_after_this(T val) {
            if (current == nullptr) return;

            auto nd = std::make_unique<Node<T>>
                (std::move(val),std::move(current->next), current);

            if (nd -> next) {
                nd -> next -> prev = nd.get();
            } else {
                list.tail = nd.get();
            }
            current -> next = std::move(nd);
            ++list.len;
        }
    };

    auto begin() {
        if (empty()) return iterator(*this);
        iterator it(head.get(), *this);
        return it;
    }

    auto end() {
        if (empty()) return iterator(*this);
        iterator it(tail, *this);
        return ++it; // именно так. Если --, то будет последний элемент
    }

    auto size() const {
        return len;
    }

    auto empty() const {
        return len == 0uz;
    }
};