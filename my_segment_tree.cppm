module;

#include <memory>
#include <utility>
#include <vector>
#include <optional>

export module my_segment_trie;
using namespace std;

template<typename T>
static auto get_nd_data(pair<T, size_t> x, pair<T, size_t> y) {
    return x > y ? x : y;
}

template<typename T>
struct node {
    T data;
    size_t idx;
    size_t l, r;
    shared_ptr<node> left = nullptr;
    shared_ptr<node> right = nullptr;

    explicit node(T d, size_t i) : data(std::move(d)), idx(i), l(i), r(i) {}

    explicit node(shared_ptr<node> n) : node(n -> data, n -> idx) {
            left = std::move(n);
            l = left -> l;
            r = left -> r;
    }

    explicit node(shared_ptr<node> n1, shared_ptr<node> n2) :
        left(std::move(n1)), right(std::move(n2)),
        data(left -> data >= right -> data ? left -> data : right -> data),
        idx (left -> data >= right -> data ? left -> idx  : right -> idx),
        l(left -> l), r(right -> r) {}
};


export template<typename T>
class SegmentTree {
    shared_ptr<node<T>> root;

public:
    explicit SegmentTree(const vector<T>& v) {
        const size_t len0 = v.size();
        if (len0 == 0) {
            root = nullptr;
            return;
        }

        vector<shared_ptr<node<T>>> leaves;
        leaves.reserve(len0);
        for (size_t i = 0; i < len0; ++i)
            leaves.push_back(make_shared<node<T>>(v[i], i));

        size_t len = len0;
        while (len > 1) {
            size_t i = 1;
            for (; i < len; i += 2)
                leaves[i / 2] = make_shared<node<T>>(leaves[i - 1], leaves[i]);
            if (i == len)
                leaves[i / 2] = make_shared<node<T>>(leaves[i - 1]);
            len = len / 2 + len % 2;
        }
        root = leaves[0];
    }

    [[nodiscard]] optional<pair<T, size_t>> get_max(size_t l, size_t r) const {
        return query(root, l, r);
    }

private:
    static optional<pair<T, size_t>>
    query(const shared_ptr<node<T>>& v, const size_t ql, const size_t qr) {
        if (!v || v -> r < ql || v -> l > qr) {
            return nullopt;
        }

        if (ql <= v -> l && v -> r <= qr) {
            return make_pair(v -> data, v -> idx);
        }

        auto left_res  = query(v -> left,  ql, qr);
        auto right_res = query(v -> right, ql, qr);

        if (left_res == nullopt)  return right_res;
        if (right_res == nullopt) return left_res;

        return get_nd_data(std::move(left_res).value(), std::move(right_res).value());
    }
};