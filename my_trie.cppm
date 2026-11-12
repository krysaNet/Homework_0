module;

#include <iostream>
#include <memory>
#include <unordered_map>

export module my_trie;
using namespace std;

export class Trie {
    struct node {
        bool is_end_of_word;
        unordered_map<char, unique_ptr<node>> children;

        explicit node(bool is_end = false) : is_end_of_word(is_end) {}
    };

    unique_ptr<node> root = make_unique<node>();
    size_t len = 0;

    [[nodiscard]] const node* get_last_simbol(const string& s) const {
        const node* curr = root.get();
        for (char c : s) {
            auto it = curr -> children.find(c);
            if (it == curr -> children.end()) return nullptr;
            curr = it -> second.get();
        }
        return curr;
    }

public:
    [[nodiscard]] size_t size() const {
        return len;
    }

    void insert(const string& s) {
        node* curr = root.get();
        for (char c : s) {
            auto& next_node = curr -> children[c];
            if (!next_node) {
                next_node = make_unique<node>();
            }
            curr = next_node.get();
        }

        if (!curr -> is_end_of_word) {
            curr -> is_end_of_word = true;
            ++len;
        }
    }

    [[nodiscard]] bool starts_with(const string& s) const { // does anything have this prefix
        return get_last_simbol(s) != nullptr;
    }

    [[nodiscard]] bool contains(const string& s) const {
        if (auto n = get_last_simbol(s)) return n -> is_end_of_word;
        return false;
    }
};