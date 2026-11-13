module;

#include <iostream>
#include <memory>

export module my_list;

template<typename T>
    struct Node {
    T data;
    std::unique_ptr<Node<T>> next;
    Node<T>* prev;

    Node(auto val, std::unique_ptr<Node<T>> next = nullptr, Node<T>* prev = nullptr) :
        data(std::move(val)), next(std::move(next)), prev(prev) {}
};

export template<typename T>
class List {
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
        Node<T>* raw = x.get();

        if (raw -> next) {
            raw -> next -> prev = raw;
        }
        else {
            tail = raw;
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

        auto& get_list() {
            return list;
        }

        auto operator == (const iterator& it) const {
            return current == it.current;
        }

        auto operator != (const iterator& it) const {
            return current != it.current;
        }

        auto erase() {
            if (!current) {
                std::cout << "Cannot erase end iterator" << std::endl;
                exit(1);
            }
            if (current -> prev == nullptr) list.pop_front();
            else if (current->next.get() == nullptr) list.pop_back();
            else {
                current -> next -> prev = current -> prev;
                current -> prev -> next = std::move(current -> next);
                --list.len;
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
            if (!current) current = list.tail;
            else current = current -> prev;
            return *this;
        }

        void push_after_this(T val) {
            if (current == nullptr) return;

            auto nd = std::make_unique<Node<T>>
                (std::move(val),std::move(current -> next), current);

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
    auto last() {
        return iterator(*tail, *this);
    }

    auto it(std::size_t idx) {
        if (idx >= len) return end();

        if (idx <= len / 2) {
            iterator res(head.get(), *this);
            for (std::size_t i = 0; i < idx; ++i) ++res;
            return res;
        }

        iterator res(tail, *this);
        for (std::size_t i = len - 1; i > idx; --i) --res;
        return res;
    }

    void erase(iterator& it) {
        if (&it.get_list() != this) {
            std::cout << "Cannot use iterator of another list" << std::endl;
            exit(1);
        }
        it.erase();
    }
    void erase(const std::size_t idx) {
        if (idx >= len) {
            std::cout << "Out of range" << std::endl;
            exit(1);
        }
        auto i = it(idx);
        erase(i);
    }

    void insert_before(T val, iterator it) {
        if (&it.get_list() != this) {
            std::cout << "Cannot use iterator of another list" << std::endl;
            exit(1);
        }
        if (it == end()) {
            push_back(std::move(val));
        }
        else if (it == begin()) {
            push_front(std::move(val));
        }
        else {
            --it;
            it.push_after_this(std::move(val));
        }
    }
    void insert_before(T val, const std::size_t idx) {
        if (idx > len) {
            std::cout << "Out of range" << std::endl;
            exit(1);
        }
        insert_before(std::move(val), it(idx));
    }

    auto size() const {
        return len;
    }
    auto empty() const {
        return len == 0uz;
    }
};