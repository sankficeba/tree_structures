#include <iostream>
#include <SFML/Graphics.hpp>
#include <ctime>
#include <algorithm>
#include <map>

sf::Font font;
std::map<std::pair<int, int>, int> avl_keys, rb_keys, dd_keys, splay_keys;

struct AVL_Node
{
    int key;
    int height;
    AVL_Node* left;
    AVL_Node* right;
    int cnt;
    AVL_Node(int k) { key = k; left = right = nullptr; height = 1; cnt = 1; }
};
AVL_Node* AVL_t = nullptr;
int AVL_cnt(AVL_Node* p) {
    return p ? p->cnt : 0;
}
int AVL_height(AVL_Node* p) {
    return p ? p->height : 0;
}
int AVL_bfactor(AVL_Node* p) {
    return AVL_height(p->right) - AVL_height(p->left);
}
void AVL_fixheight(AVL_Node* p)
{
    int hl = AVL_height(p->left);
    int hr = AVL_height(p->right);
    p->height = (hl > hr ? hl : hr) + 1;
}
void AVL_update_cnt(AVL_Node* p) {
    if (p == nullptr) return;
    p->cnt = AVL_cnt(p->left) + AVL_cnt(p->right) + 1;
}
AVL_Node* AVL_rotate_right(AVL_Node* p) {
    AVL_Node* q = p->left;
    p->left = q->right;
    q->right = p;
    AVL_fixheight(p);
    AVL_fixheight(q);
    return q;
}
AVL_Node* AVL_rotate_left(AVL_Node* p) {
    AVL_Node* q = p->right;
    p->right = q->left;
    q->left = p;
    AVL_fixheight(p);
    AVL_fixheight(q);
    return q;
}
AVL_Node* AVL_balance(AVL_Node* p) {
    AVL_fixheight(p);
    if (AVL_bfactor(p) == 2) {
        if (AVL_bfactor(p->right) < 0) p->right = AVL_rotate_right(p->right);
        return AVL_rotate_left(p);
    }
    else if (AVL_bfactor(p) == -2) {
        if (AVL_bfactor(p->left) > 0) p->left = AVL_rotate_left(p->left);
        return AVL_rotate_right(p);
    }
    return p;
}
AVL_Node* AVL_find_min(AVL_Node* p) {
    return p->left ? AVL_find_min(p->left) : p;
}
AVL_Node* AVL_remove_min(AVL_Node* p)
{
    if (p->left == 0) return p->right;
    p->left = AVL_remove_min(p->left);
    return AVL_balance(p);
}
AVL_Node* AVL_ins(AVL_Node* p, int key) {
    if (!p) return new AVL_Node(key);
    if (key < p->key)
    {
        p->left = AVL_ins(p->left, key);
    }
    else
    {
        p->right = AVL_ins(p->right, key);
    }
    return AVL_balance(p);
}
AVL_Node* AVL_rem(AVL_Node* p, int key) {
    if (!p) return nullptr;
    if (key < p->key) p->left = AVL_rem(p->left, key);
    else if (key > p->key) p->right = AVL_rem(p->right, key);
    else
    {
        AVL_Node* q = p->left;
        AVL_Node* r = p->right;
        delete p;
        if (!r) return q;
        AVL_Node* min = AVL_find_min(r);
        min->right = AVL_remove_min(r);
        min->left = q;
        return AVL_balance(min);
    }
    return AVL_balance(p);
}
void AVL_order(AVL_Node* p) {
    if (p == nullptr) return;
    AVL_order(p->left);
    AVL_order(p->right);
    AVL_update_cnt(p);
}
void AVL_insert(int key) {
    AVL_t = AVL_ins(AVL_t, key);
    AVL_order(AVL_t);
}
void AVL_remove(int key) {
    AVL_t = AVL_rem(AVL_t, key);
    AVL_order(AVL_t);
}

struct RB_Node {
    int key;
    int height;
    bool red;
    int cnt;
    RB_Node* left;
    RB_Node* right;
    RB_Node* pred;
    RB_Node(int k, RB_Node* pr) { key = k; pred = pr; left = right = nullptr; height = 1; red = 1; cnt = 1; }
};
RB_Node* RB_t = nullptr;
int RB_height(RB_Node* p) {
    return p ? p->height : 0;
}
int RB_cnt(RB_Node* p) {
    return p ? p->cnt : 0;
}
void RB_update_cnt(RB_Node* p) {
    if (p == nullptr) return;
    p->cnt = RB_cnt(p->left) + RB_cnt(p->right) + 1;
}
void RB_fixheight(RB_Node*& p)
{
    if (p == nullptr)return;
    int hl = RB_height(p->left);
    int hr = RB_height(p->right);
    p->height = (hl > hr ? hl : hr) + 1;
}
void RB_ins_balance(RB_Node* n) {
    if (n->pred == nullptr) {
        n->red = 0;
        return;
    }
    if (n->pred->red == 0) return;
    if (n->pred->red == 1 && n->pred->pred->left != nullptr && n->pred->pred->right != nullptr && n->pred->pred->left->red == 1 && n->pred->pred->right->red == 1)
    {
        RB_Node* parent = n->pred;
        RB_Node* uncle = (n->pred->pred->left == parent) ? n->pred->pred->right : n->pred->pred->left;
        parent->red = 0;
        uncle->red = 0;
        n->pred->pred->red = 1;
        RB_ins_balance(n->pred->pred);
        return;
    }
    RB_Node* parent = n->pred;
    RB_Node* granny = parent->pred;
    if (granny->left == parent && parent->left == n) {//4 1
        RB_Node* uncle = granny->right;
        granny->left = parent->right;
        granny->red = 1;
        if (granny->left != nullptr) granny->left->pred = granny;
        parent->pred = granny->pred;
        granny->pred = parent;
        parent->right = granny;
        parent->red = 0;
        if (parent->pred) {
            if (parent->pred->right == granny) parent->pred->right = parent;
            else
            {
                parent->pred->left = parent;
            }
        }
        RB_fixheight(granny);
        RB_fixheight(parent);
        return;
    }
    if (granny->right == parent && parent->right == n) {//4 2
        RB_Node* uncle = granny->left;
        granny->right = parent->left;
        granny->red = 1;
        if (granny->right != nullptr) granny->right->pred = granny;
        parent->pred = granny->pred;
        granny->pred = parent;
        parent->left = granny;
        parent->red = 0;
        if (parent->pred) {
            if (parent->pred->right == granny) parent->pred->right = parent;
            else
            {
                parent->pred->left = parent;
            }
        }
        RB_fixheight(granny);
        RB_fixheight(parent);
        return;
    }
    if (granny->left == parent && parent->right == n) {// 5 1
        granny->left = n;
        n->pred = granny;
        parent->right = n->left;
        if (n->left) n->left->pred = parent;
        parent->pred = n;
        n->left = parent;
        RB_fixheight(parent);
        RB_fixheight(n);
        RB_fixheight(granny);
        //4 1
        RB_Node* uncle = granny->right;
        granny->left = n->right;
        granny->red = 1;
        if (granny->left != nullptr) granny->left->pred = granny;
        n->pred = granny->pred;
        granny->pred = n;
        if (n->pred != nullptr) {
            if (n->pred->left == granny) {
                n->pred->left = n;
            }
            else {
                n->pred->right = n;
            }
        }
        n->right = granny;
        n->red = 0;
        RB_fixheight(granny);
        RB_fixheight(n);
        return;
    }
    if (granny->right == parent && parent->left == n) {// 5 2
        granny->right = n;
        n->pred = granny;
        parent->left = n->right;
        if (n->right) n->right->pred = parent;
        parent->pred = n;
        n->right = parent;
        RB_fixheight(parent);
        RB_fixheight(n);
        RB_fixheight(granny);
        //4 2
        RB_Node* uncle = granny->left;
        granny->right = n->left;
        granny->red = 1;
        if (granny->right != nullptr) granny->right->pred = granny;
        n->pred = granny->pred;
        granny->pred = n;
        if (n->pred) {
            if (n->pred->left == granny) {
                n->pred->left = n;
            }
            else {
                n->pred->right = n;
            }
        }
        n->left = granny;
        n->red = 0;
        RB_fixheight(granny);
        RB_fixheight(n);
        return;
    }
}
void RB_ins(RB_Node*& p, int key, RB_Node* pred) {
    if (!p && !pred) {
        p = new RB_Node(key, pred);
        RB_ins_balance(p);
        return;
    }
    if (key < p->key)
    {
        if (p->left == nullptr)
        {
            p->left = new RB_Node(key, p);
            RB_ins_balance(p->left);
        }
        else
        {
            RB_ins(p->left, key, p->left);
        }
    }
    else
    {
        if (p->right == nullptr)
        {
            p->right = new RB_Node(key, p);
            RB_ins_balance(p->right);
        }
        else
        {
            RB_ins(p->right, key, p->right);
        }
    }
    RB_fixheight(p);
}
void RB_rem_balance(RB_Node* n) {
    if (!n->pred) return;
    RB_Node* parent = n->pred;
    RB_Node* sibling;
    (parent->left != n) ? sibling = parent->left : sibling = parent->right;
    RB_Node* sibling_left = sibling->left;
    RB_Node* siblibg_right = sibling->right;

    if (parent->red == 0 && sibling->red == 0 && (!sibling_left || sibling_left->red == 0)&& (!siblibg_right || siblibg_right->red == 0))
    {
        sibling->red = 1;
        RB_rem_balance(parent);
    }
    else if (parent->red == 1 && (!sibling_left || sibling_left->red == 0) && (!siblibg_right || siblibg_right->red == 0)) {
        parent->red = 0;
        sibling->red = 1;
        return;
    }
    else if (sibling->red == 0 && siblibg_right && siblibg_right->red == 1 && n == parent->left) {//4 - nps
        sibling->red = parent->red;
        parent->red = 0;
        siblibg_right->red = 0;
        parent->right = sibling->left;
        if (sibling->left) sibling->left->pred = parent;
        sibling->pred = parent->pred;
        if (sibling->pred) {
            if (sibling->pred->left == parent)
            {
                sibling->pred->left = sibling;
            }
            else
            {
                sibling->pred->right = sibling;
            }
        }
        parent->pred = sibling;
        sibling->left = parent;
        return;
    }
    else if (sibling->red == 0 && sibling_left && sibling_left->red == 1 && (!siblibg_right || siblibg_right->red == 0) && n == parent->left) {//5 - nps
        sibling_left->red = 0;
        sibling->red = 1;
        parent->right = sibling_left;
        sibling_left->pred = parent;
        sibling->left = sibling_left->right;
        if (sibling->left) sibling->left->pred = sibling;
        sibling_left->right = sibling;
        sibling->pred = sibling_left;
        RB_rem_balance(n);
        return;
    }
    else if (sibling->red == 1 && (!sibling_left || sibling_left->red == 0) && (!siblibg_right || siblibg_right->red == 0) && n == parent->left) {//6 - nps
        parent->red = 1;
        sibling->red = 0;
        parent->right = sibling_left;
        if (sibling_left) sibling_left->pred = parent;
        sibling->pred = parent->pred;
        if (sibling->pred) {
            if (sibling->pred->left == parent) {
                sibling->pred->left = sibling;
            }
            else
            {
                sibling->pred->right = sibling;
            }
        }
        sibling->left = parent;
        parent->pred = sibling;
        RB_rem_balance(n);
        return;
    }
    else if (sibling->red == 0 && sibling_left && sibling_left->red == 1 && n == parent->right) {//4 - spn
        sibling->red = parent->red;
        parent->red = 0;
        sibling_left->red = 0;
        parent->left = sibling->right;
        if (sibling->right) sibling->right->pred = parent;
        sibling->pred = parent->pred;
        if (sibling->pred) {
            if (sibling->pred->left == parent)
            {
                sibling->pred->left = sibling;
            }
            else
            {
                sibling->pred->right = sibling;
            }
        }
        parent->pred = sibling;
        sibling->right = parent;
        return;
    }
    else if (sibling->red == 0 && (!sibling_left || sibling_left->red == 0) && siblibg_right && siblibg_right->red == 1 && n == parent->right) {//5 - spn
        sibling->red = 1;
        siblibg_right->red = 0;
        parent->left = siblibg_right;
        siblibg_right->pred = parent;
        sibling->right = siblibg_right->left;
        if (sibling->right) sibling->right->pred = sibling;
        siblibg_right->left = sibling;
        sibling->pred = siblibg_right;
        RB_rem_balance(n);
        return;
    }
    else if (sibling->red == 1 && (!sibling_left || sibling_left->red == 0) && (!siblibg_right || siblibg_right->red == 0) && n == parent->right) {//6 - spn
        parent->red = 1;
        sibling->red = 0;
        parent->left = siblibg_right;
        if (siblibg_right) siblibg_right->pred = parent;
        sibling->pred = parent->pred;
        if (sibling->pred) {
            if (sibling->pred->left == parent) {
                sibling->pred->left = sibling;
            }
            else
            {
                sibling->pred->right = sibling;
            }
        }
        sibling->right = parent;
        parent->pred = sibling;
        RB_rem_balance(n);
        return;
    }
}
RB_Node* RB_find_min(RB_Node* p) {
    return p->left ? RB_find_min(p->left) : p;
}
void RB_rem(RB_Node* t, int key) {
    if (!t) return;
    if (!t->pred && !t->left && !t->right) {
        RB_t = nullptr;
        return;
    }
    if (key < t->key) return RB_rem(t->left, key);
    else if (key > t->key) return RB_rem(t->right, key);
    else
    {
        RB_Node* r = nullptr;
        if (t->right) r = t->right;
        RB_Node* n = r?RB_find_min(r):t;
        std::swap(t->key, n->key);

        if (n->red == 1 && !n->left && !n->right) {
            if (n->pred) {
                if (n->pred->left == n) n->pred->left = nullptr;
                else n->pred->right = nullptr;
            }
            delete n;
            return;
        }
        else if (n->red == 0 && (n->left == nullptr && n->right && n->right->red == 1||n->right == nullptr && n->left && n->left->red == 1)) {
            RB_Node* tmp = (n->left) ? n->left : n->right;
            std::swap(tmp->key, n->key);
            n->right = nullptr; n->left = nullptr;
            delete tmp;
            return;
        }
        else {
            RB_rem_balance(n);
            if (n->pred) {
                if (n->pred->left == n) n->pred->left = nullptr;
                else n->pred->right = nullptr;
                delete n;
            }
        }
    }
}
void RB_order(RB_Node* p) {
    if (p == nullptr) return;
    RB_order(p->left);
    RB_order(p->right);
    RB_update_cnt(p);
}
void RB_insert(int key) {
    RB_ins(RB_t, key, nullptr);
    while (RB_t->pred != nullptr) RB_t = RB_t->pred;
    RB_order(RB_t);
}
void RB_remove(int key) {
    RB_rem(RB_t, key);
    if (RB_t) while (RB_t->pred != nullptr) RB_t = RB_t->pred;
    RB_order(RB_t);
}

struct DD_Node {
    int key, priority;
    DD_Node* left, * right;
    int height;
    int cnt;
};
DD_Node* DD_t = nullptr;
int DD_cnt(DD_Node* p) {
    return p ? p->cnt : 0;
}
int DD_height(DD_Node* t) {
    return t ? t->height : 0;
}
void DD_update_height(DD_Node* t) {
    if (t == nullptr) return;
    t->height = 1 + DD_height(t->left) + DD_height(t->right);
    t->cnt = DD_cnt(t->left) + DD_cnt(t->right) + 1;
}
void DD_merge(DD_Node*& t, DD_Node* l, DD_Node* r) {
    if (!l || !r)
        t = l ? l : r;
    else if (l->priority < r->priority)
        DD_merge(l->right, l->right, r), t = l;
    else
        DD_merge(r->left, l, r->left), t = r;
    DD_update_height(t);
}
void DD_split(DD_Node* t, DD_Node*& l, DD_Node*& r, int key) {
    if (!t) {
        l = r = nullptr;
        return;
    }
    if (t->key >= key)
        DD_split(t->left, l, t->left, key), r = t;
    else
        DD_split(t->right, t->right, r, key), l = t;
    DD_update_height(t);
}
void DD_ins(DD_Node*& t, DD_Node* new_item) {
    if (!t)
    {
        t = new_item;
    }
    else if (new_item->priority < t->priority) {
        DD_split(t, new_item->left, new_item->right, new_item->key), t = new_item;
    }
    else
    {
        DD_ins(new_item->key < t->key ? t->left : t->right, new_item);
    }
}
void DD_rem(DD_Node*& t, int key) {
    if (!t) return;
    if (t->key == key)
        DD_merge(t, t->left, t->right);
    else
        DD_rem(key < t->key ? t->left : t->right, key);
}
void DD_output(DD_Node* t) {
    if (t == nullptr) return;
    DD_output(t->left);
    std::cout << t->key << ' ';
    DD_output(t->right);
}
void DD_order(DD_Node* p) {
    if (p == nullptr) return;
    DD_order(p->left);
    DD_order(p->right);
    DD_update_height(p);
}
void DD_insert(int key) {
    DD_Node* new_item = new DD_Node;
    new_item->priority = std::rand() % 1000;
    new_item->key = key;
    new_item->left = nullptr;
    new_item->right = nullptr;
    new_item->cnt = 1;
    DD_ins(DD_t, new_item);
    DD_order(DD_t);
}
void DD_remove(int key) {
    DD_rem(DD_t, key);
}

struct Splay_Node {
    int key;
    Splay_Node* left;
    Splay_Node* right;
    Splay_Node* parent;
    int cnt;
    Splay_Node(int k, Splay_Node* pr) { key = k; parent = pr; left = right = nullptr; cnt = 1; }
};
int Splay_cnt(Splay_Node* p) {
    return p ? p->cnt : 0;
}
void Splay_update_cnt(Splay_Node* p) {
    if (p == nullptr) return;
    p->cnt = Splay_cnt(p->left) + Splay_cnt(p->right) + 1;
}
Splay_Node* Splay_t = nullptr;
Splay_Node* Splay_left_zig(Splay_Node* x) {
    Splay_Node* y = x->parent;
    y->left = x->right;
    x->right = y;
    if (y->left) y->left->parent = y;
    x->parent = y->parent;
    y->parent = x;
    return x;
}
Splay_Node* Splay_right_zig(Splay_Node* x) {
    Splay_Node* y = x->parent;
    y->right = x->left;
    x->left = y;
    if (y->right) y->right->parent = y;
    x->parent = y->parent;
    y->parent = x;
    return x;
}
Splay_Node* Splay_left_zig_zig(Splay_Node* x) {
    Splay_Node* y = x->parent;
    Splay_Node* z = y->parent;
    y->left = x->right;
    z->left = y->right;
    x->right = y;
    y->right = z;
    if (y->left) y->left->parent = y;
    if (z->left) z->left->parent = z;
    x->parent = z->parent;
    y->parent = x;
    z->parent = y;
    if (x->parent != nullptr)
    {
        (x->parent->left == z) ? x->parent->left = x : x->parent->right = x;
    }
    return x;
}
Splay_Node* Splay_right_zig_zig(Splay_Node* x) {
    Splay_Node* y = x->parent;
    Splay_Node* z = y->parent;
    y->right = x->left;
    z->right = y->left;
    x->left = y;
    y->left = z;
    if (y->right) y->right->parent = y;
    if (z->right) z->right->parent = z;
    x->parent = z->parent;
    y->parent = x;
    z->parent = y;
    if (x->parent != nullptr)
    {
        (x->parent->left == z) ? x->parent->left = x : x->parent->right = x;
    }
    return x;
}
Splay_Node* Splay_left_zig_zag(Splay_Node* x) {
    Splay_Node* y = x->parent;
    Splay_Node* z = y->parent;
    z->left = x->right;
    y->right = x->left;
    x->left = y;
    x->right = z;
    if (y->right) y->right->parent = y;
    if (z->left) z->left->parent = z;
    x->parent = z->parent;
    y->parent = x;
    z->parent = x;
    if (x->parent != nullptr)
    {
        (x->parent->left == z) ? x->parent->left = x : x->parent->right = x;
    }
    return x;
}
Splay_Node* Splay_right_zig_zag(Splay_Node* x) {
    Splay_Node* y = x->parent;
    Splay_Node* z = y->parent;
    z->right = x->left;
    y->left = x->right;
    x->left = z;
    x->right = y;
    if (z->right) z->right->parent = z;
    if (y->left) y->left->parent = y;
    x->parent = z->parent;
    z->parent = x;
    y->parent = x;
    if (x->parent != nullptr)
    {
        (x->parent->left == z) ? x->parent->left = x : x->parent->right = x;
    }
    return x;
}
Splay_Node* Splay_splay(Splay_Node* p) {
    if (!p || p->parent == nullptr) return p;
    if (p->parent->parent == nullptr) {
        if (p->parent->left == p) {
            p = Splay_left_zig(p);
            return p;
        }
        else {
            p = Splay_right_zig(p);
            return p;
        }
    }
    else if (p->parent->parent->left == p->parent) {
        if (p->parent->left == p) {
            p = Splay_left_zig_zig(p);
            return Splay_splay(p);
        }
        else {
            p = Splay_left_zig_zag(p);
            return Splay_splay(p);
        }
    }
    else
    {
        if (p->parent->left == p) {
            p = Splay_right_zig_zag(p);
            return Splay_splay(p);
        }
        else {
            p = Splay_right_zig_zig(p);
            return Splay_splay(p);
        }
    }
}
Splay_Node* Splay_ins(Splay_Node* p, int key, Splay_Node* parent) {
    if (!p && !parent) {
        p = new Splay_Node(key, parent);
        return p;
    }
    if (key < p->key)
    {
        if (p->left == nullptr) {
            p->left = new Splay_Node(key, p);
            return p = Splay_splay(p->left);
        }
        else Splay_ins(p->left, key, p);
    }
    else
    {
        if (p->right == nullptr) {
            p->right = new Splay_Node(key, p);
            return  p = Splay_splay(p->right);
        }
        else Splay_ins(p->right, key, p);
    }
}
Splay_Node* Splay_max(Splay_Node* p) {
    if (!p) return nullptr;
    if (p->right == nullptr) return p;
    Splay_max(p->right);
}
Splay_Node* Splay_rem(Splay_Node* p, int key) {
    if (!p) {
        p = nullptr;
        return nullptr;
    }
    if (key < p->key) {
        if (!p->left) {
            return p = Splay_splay(p);
        }
        return Splay_rem(p->left, key);
    }
    else if (key > p->key) {
        if (!p->right) {
            return p = Splay_splay(p);
        }
        return Splay_rem(p->right, key);
    }
    else
    {
        p = Splay_splay(p);
        Splay_Node* l = nullptr;
        if (p->left) {
            l = p->left;
            l->parent = nullptr;
        }
        Splay_Node* r = nullptr;
        if (p->right) {
            r = p->right;
            r->parent = nullptr;
        }
        delete p;
        l = Splay_max(l);
        l = Splay_splay(l);
        if (l == nullptr) {
            p = r;
            return p;
        }
        l->right = r;
        if (r) r->parent = l;
        p = l;
        return p;
    }
}
void Splay_order(Splay_Node* p) {
    if (p == nullptr) return;
    Splay_order(p->left);
    Splay_order(p->right);
    Splay_update_cnt(p);
}
void Splay_insert(int key) {
    Splay_t = Splay_ins(Splay_t, key, nullptr);
    Splay_order(Splay_t);
}
void Splay_remove(int key) {
    Splay_t = Splay_rem(Splay_t, key);
    Splay_order(Splay_t);
}

int Zoom = 40;
int GLOBALX = 0;
int GLOBALY = 0;

void draw_avl(AVL_Node* p, int cur_x, int cur_y, sf::RenderWindow& window, int last_x, int last_y, bool first = false) {
    if (!p) return;
    int delta_x;
    if (p->left == nullptr)
    {
        delta_x = 0;
    }
    else
    {
        delta_x = AVL_cnt(p->left);
    }

    sf::RectangleShape rect;
    rect.setSize(sf::Vector2f(Zoom, Zoom));
    rect.setPosition(sf::Vector2f((cur_x + delta_x) * Zoom + GLOBALX, GLOBALY + cur_y * Zoom * 2));
    avl_keys[{(cur_x + delta_x), cur_y * 2}] = p->key;
    rect.setFillColor(sf::Color::Black);
    sf::Font font;
    font.loadFromFile("C:/WINDOWS/Fonts/arial.ttf");

    sf::Text text;
    text.setFont(font);
    text.setString(std::to_string(p->key));
    text.setCharacterSize(Zoom/3);
    text.setFillColor(sf::Color::White);
    text.setPosition(sf::Vector2f((cur_x + delta_x) * Zoom + GLOBALX + Zoom / 25, GLOBALY + cur_y * Zoom * 2  + Zoom / 3));
    
    window.draw(rect);
    window.draw(text);
    

    sf::Vertex line[] =
    {
     sf::Vertex(sf::Vector2f(last_x + Zoom/2, last_y + Zoom)),
     sf::Vertex(sf::Vector2f((cur_x + delta_x) * Zoom + GLOBALX + Zoom / 2, GLOBALY + cur_y * Zoom * 2))
    };
    line->color = sf::Color::Red;
    (line + 1)->color = sf::Color::Red;
    if (first) {
        window.draw(line, 2, sf::Lines);
    }

    draw_avl(p->left, cur_x, cur_y + 1, window, (cur_x + delta_x) * Zoom + GLOBALX, GLOBALY + cur_y * Zoom * 2, true);
    draw_avl(p->right, cur_x + delta_x + 1, cur_y + 1, window, (cur_x + delta_x) * Zoom + GLOBALX, GLOBALY + cur_y * Zoom * 2, true);
}
void draw_rb(RB_Node* p, int cur_x, int cur_y, sf::RenderWindow& window, int last_x, int last_y, bool first = false) {
    if (!p) return;
    int delta_x;
    if (p->left == nullptr)
    {
        delta_x = 0;
    }
    else
    {
        delta_x = RB_cnt(p->left);
    }

    sf::RectangleShape rect;
    rect.setSize(sf::Vector2f(Zoom, Zoom));
    rect.setPosition(sf::Vector2f((cur_x + delta_x) * Zoom + GLOBALX, GLOBALY + cur_y * Zoom * 2));
    rb_keys[{(cur_x + delta_x), cur_y * 2}] = p->key;
    p->red == 1? rect.setFillColor(sf::Color::Red): rect.setFillColor(sf::Color::Black);
    sf::Font font;
    font.loadFromFile("C:/WINDOWS/Fonts/arial.ttf");

    sf::Text text;
    text.setFont(font);
    text.setString(std::to_string(p->key));
    text.setCharacterSize(Zoom / 3);
    text.setFillColor(sf::Color::White);
    text.setPosition(sf::Vector2f((cur_x + delta_x) * Zoom + GLOBALX + Zoom / 25, GLOBALY + cur_y * Zoom * 2 + Zoom / 3));

    window.draw(rect);
    window.draw(text);

    sf::Vertex line[] =
    {
     sf::Vertex(sf::Vector2f(last_x + Zoom / 2, last_y + Zoom)),
     sf::Vertex(sf::Vector2f((cur_x + delta_x) * Zoom + GLOBALX + Zoom / 2, GLOBALY + cur_y * Zoom * 2))
    };
    line->color = sf::Color::Black;
    (line + 1)->color = sf::Color::Black;
    if (first) {
        window.draw(line, 2, sf::Lines);
    }

    draw_rb(p->left, cur_x, cur_y + 1, window, (cur_x + delta_x) * Zoom + GLOBALX, GLOBALY + cur_y * Zoom * 2, true);
    draw_rb(p->right, cur_x + delta_x + 1, cur_y + 1, window, (cur_x + delta_x) * Zoom + GLOBALX, GLOBALY + cur_y * Zoom * 2, true);
}
void draw_dd(DD_Node* p, int cur_x, int cur_y, sf::RenderWindow& window, int last_x, int last_y, bool first = false) {
    if (!p) return;
    int delta_x;
    if (p->left == nullptr)
    {
        delta_x = 0;
    }
    else
    {
        delta_x = DD_cnt(p->left);
    }

    sf::RectangleShape rect;
    rect.setSize(sf::Vector2f(Zoom, Zoom));
    rect.setPosition(sf::Vector2f((cur_x + delta_x) * Zoom + GLOBALX, GLOBALY + cur_y * Zoom * 2));
    dd_keys[{(cur_x + delta_x), cur_y * 2}] = p->key;
    rect.setFillColor(sf::Color::Black);
    sf::Font font;
    font.loadFromFile("C:/WINDOWS/Fonts/arial.ttf");

    sf::Text text;
    text.setFont(font);
    text.setString(std::to_string(p->key) + '\n' + std::to_string(p->priority));
    text.setCharacterSize(Zoom / 3);
    text.setFillColor(sf::Color::White);
    text.setPosition(sf::Vector2f((cur_x + delta_x) * Zoom + GLOBALX + Zoom / 25, GLOBALY + cur_y * Zoom * 2 + Zoom / 5));

    window.draw(rect);
    window.draw(text);


    sf::Vertex line[] =
    {
     sf::Vertex(sf::Vector2f(last_x + Zoom / 2, last_y + Zoom)),
     sf::Vertex(sf::Vector2f((cur_x + delta_x) * Zoom + GLOBALX + Zoom / 2, GLOBALY + cur_y * Zoom * 2))
    };
    line->color = sf::Color::Red;
    (line + 1)->color = sf::Color::Red;
    if (first) {
        window.draw(line, 2, sf::Lines);
    }

    draw_dd(p->left, cur_x, cur_y + 1, window, (cur_x + delta_x) * Zoom + GLOBALX, GLOBALY + cur_y * Zoom * 2, true);
    draw_dd(p->right, cur_x + delta_x + 1, cur_y + 1, window, (cur_x + delta_x) * Zoom + GLOBALX, GLOBALY + cur_y * Zoom * 2, true);
}
void draw_splay(Splay_Node* p, int cur_x, int cur_y, sf::RenderWindow& window, int last_x, int last_y, bool first = false) {
    if (!p) return;
    int delta_x;
    if (p->left == nullptr)
    {
        delta_x = 0;
    }
    else
    {
        delta_x = Splay_cnt(p->left);
    }

    sf::RectangleShape rect;
    rect.setSize(sf::Vector2f(Zoom, Zoom));
    rect.setPosition(sf::Vector2f((cur_x + delta_x) * Zoom + GLOBALX, GLOBALY + cur_y * Zoom * 2));
    splay_keys[{(cur_x + delta_x), cur_y * 2}] = p->key;
    rect.setFillColor(sf::Color::Black);
    sf::Font font;
    font.loadFromFile("C:/WINDOWS/Fonts/arial.ttf");

    sf::Text text;
    text.setFont(font);
    text.setString(std::to_string(p->key));
    text.setCharacterSize(Zoom / 3);
    text.setFillColor(sf::Color::White);
    text.setPosition(sf::Vector2f((cur_x + delta_x) * Zoom + GLOBALX + Zoom / 25, GLOBALY + cur_y * Zoom * 2 + Zoom / 3));

    window.draw(rect);
    window.draw(text);


    sf::Vertex line[] =
    {
     sf::Vertex(sf::Vector2f(last_x + Zoom / 2, last_y + Zoom)),
     sf::Vertex(sf::Vector2f((cur_x + delta_x) * Zoom + GLOBALX + Zoom / 2, GLOBALY + cur_y * Zoom * 2))
    };
    line->color = sf::Color::Red;
    (line + 1)->color = sf::Color::Red;
    if (first) {
        window.draw(line, 2, sf::Lines);
    }

    draw_splay(p->left, cur_x, cur_y + 1, window, (cur_x + delta_x) * Zoom + GLOBALX, GLOBALY + cur_y * Zoom * 2, true);
    draw_splay(p->right, cur_x + delta_x + 1, cur_y + 1, window, (cur_x + delta_x) * Zoom + GLOBALX, GLOBALY + cur_y * Zoom * 2, true);
}

void add_one_vertex() {
    sf::RenderWindow cur_window(sf::VideoMode(200, 50), "add", sf::Style::None);
    std::string value;
    sf::Text visible_value;

    while (cur_window.isOpen()) {
        sf::Event event;
        cur_window.clear(sf::Color::Green);

        visible_value.setString(value);
        visible_value.setFont(font);
        visible_value.setCharacterSize(20);
        visible_value.setFillColor(sf::Color::Black);
        visible_value.setPosition(sf::Vector2f(60, 10));

        cur_window.draw(visible_value);

        while (cur_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) cur_window.close();
            if (event.type == sf::Event::TextEntered) {
                if (event.key.code == '\r') {
                    if (value.empty()) {
                        cur_window.close();
                        continue;
                    }
                    int num = std::stoi(value);
                    AVL_insert(num);
                    RB_insert(num);
                    DD_insert(num);
                    Splay_insert(num);
                    cur_window.close();
                }
                else if (event.key.code == '\b') {
                    if (!value.empty()) value.pop_back();
                }
                else if (event.key.code >= '0' && event.key.code <= '9') {
                    if (value.size() >= 5) continue;
                    value.push_back(event.text.unicode);
                }
            }
        }

        cur_window.display();
    }
}
void add_lot_vertex() {
    sf::RenderWindow cur_window(sf::VideoMode(200, 50), "add", sf::Style::None);
    std::string value;
    sf::Text visible_value;

    while (cur_window.isOpen()) {
        sf::Event event;
        cur_window.clear(sf::Color::Green);

        visible_value.setString(value);
        visible_value.setFont(font);
        visible_value.setCharacterSize(20);
        visible_value.setFillColor(sf::Color::Black);
        visible_value.setPosition(sf::Vector2f(60, 10));

        cur_window.draw(visible_value);

        while (cur_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) cur_window.close();
            if (event.type == sf::Event::TextEntered) {
                if (event.key.code == '\r') {
                    if (value.empty()) {
                        cur_window.close();
                        continue;
                    }
                    int num = std::stoi(value);
                    for (int i = 0; i < num; i++)
                    {
                        int k = rand();
                        AVL_insert(k);
                        RB_insert(k);
                        DD_insert(k);
                        Splay_insert(k);
                    }
                    
                    cur_window.close();
                }
                else if (event.key.code == '\b') {
                    if (!value.empty()) value.pop_back();
                }
                else if (event.key.code >= '0' && event.key.code <= '9') {
                    if (value.size() >= 3) continue;
                    value.push_back(event.text.unicode);
                }
            }
        }

        cur_window.display();
    }
}
void delete_vertex(int x, int y, std::string tree) {
    x -= GLOBALX; y -= GLOBALY;
    x = x / Zoom;
    y = y / Zoom;
    int key = -1;
    if (tree == "AVL" && avl_keys.count({ x, y })) key = avl_keys[{x, y}];
    if (tree == "RB" && rb_keys.count({x, y})) key = rb_keys[{x, y}];
    if (tree == "DD" && dd_keys.count({ x, y })) key = dd_keys[{x, y}];
    if (tree == "SPLAY" && splay_keys.count({ x, y })) key = splay_keys[{x, y}];
    if (key == -1) return;
    AVL_remove(key);
    RB_remove(key);
    DD_remove(key);
    Splay_remove(key);
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(1600, 900), "CLASSES OF TREES");
    bool isMove = false;//переменная для щелчка мыши по спрайту
    float dX = 0;//корректировка движения по х
    float dY = 0;//по у

    std::string current_tree;
    bool is_avl, is_rb, is_dd, is_splay;
    bool ins_val, ins_rand;
    ins_val = ins_rand = false;
    is_avl = is_rb = is_dd = is_splay = false;
    while (window.isOpen()) {
        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        sf::Vector2f pos = window.mapPixelToCoords(pixelPos);
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::MouseButtonPressed && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                if (pixelPos.x >= 0 && pixelPos.x <= 200 &&
                    pixelPos.y >= 800 && pixelPos.y <= 900) {
                    current_tree = "AVL";
                }
                if (pixelPos.x >= 200 && pixelPos.x <= 400 &&
                    pixelPos.y >= 800 && pixelPos.y <= 900) {
                    current_tree = "RB";
                }
                if (pixelPos.x >= 400 && pixelPos.x <= 600 &&
                    pixelPos.y >= 800 && pixelPos.y <= 900) {
                    current_tree = "DD";
                }
                if (pixelPos.x >= 600 && pixelPos.x <= 800 &&
                    pixelPos.y >= 800 && pixelPos.y <= 900) {
                    current_tree = "SPLAY";
                }
                if (pixelPos.x >= 1000 && pixelPos.x <= 1480 &&
                    pixelPos.y >= 150 && pixelPos.y <= 250) {
                    ins_val = true;
                }
                if (pixelPos.x >= 1000 && pixelPos.x <= 1480 &&
                    pixelPos.y >= 400 && pixelPos.y <= 500) {
                    ins_rand = true;
                }
                if (pixelPos.x >= 0 && pixelPos.x <= 900 &&
                    pixelPos.y >= 0 && pixelPos.y <= 800)
                {
                    delete_vertex(pixelPos.x, pixelPos.y, current_tree);
                    //draw_avl(AVL_t, 0, 0, window, 0, 0);
                }
            }
            if (pixelPos.x >= 0 && pixelPos.x <= 200 &&
                pixelPos.y >= 800 && pixelPos.y <= 900) {
                is_avl = true, is_rb = false, is_dd = false, is_splay = false;
            }
            else if (pixelPos.x >= 200 && pixelPos.x <= 400 &&
                pixelPos.y >= 800 && pixelPos.y <= 900) {
                is_avl = false, is_rb = true, is_dd = false, is_splay = false;
            }
            else if (pixelPos.x >= 400 && pixelPos.x <= 600 &&
                pixelPos.y >= 800 && pixelPos.y <= 900) {
                is_avl = false, is_rb = false, is_dd = true, is_splay = false;
            }
            else if (pixelPos.x >= 600 && pixelPos.x <= 800 &&
                pixelPos.y >= 800 && pixelPos.y <= 900) {
                is_avl = false, is_rb = false, is_dd = false, is_splay = true;
            }
            else
            {
                is_avl = false, is_rb = false, is_dd = false, is_splay = false;
            }
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::MouseWheelMoved) {
                Zoom += event.mouseWheel.delta;
                std::cout << Zoom << '\n';
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.key.code == sf::Mouse::Left) {
                    std::cout << "isClicked!\n";
                    dX = pos.x - GLOBALX;
                    dY = pos.y - GLOBALY;
                    isMove = true;
                }
            }
            if (event.type == sf::Event::MouseButtonReleased) {
                if (event.key.code == sf::Mouse::Left) {
                    isMove = false;
                }
            }
            if (Zoom < 1) {
                Zoom = 1;
            }
        }
        if (isMove) {
            GLOBALX = pos.x - dX;
            GLOBALY = pos.y - dY;
            std::cout << GLOBALX << ' ' << GLOBALY << '\n';
        }

        window.clear();
       // std::cout << sf::Mouse::getPosition(window).x << ' ' << sf::Mouse::getPosition(window).y << std::endl;
        window.clear(sf::Color::White);

        sf::RectangleShape menu;
        menu.setSize(sf::Vector2f(700, 900));
        menu.setPosition(sf::Vector2f(900, 0));
        menu.setFillColor(sf::Color(0, 100, 100));

        sf::RectangleShape rect1;
        rect1.setPosition(0, 800);
        rect1.setSize(sf::Vector2f(200, 100));
        if (is_avl) 
        {
            rect1.setFillColor(sf::Color::Yellow);
        }
        else
        {
            rect1.setFillColor(sf::Color::Black);
        }
        rect1.setOutlineColor(sf::Color::Red);
        rect1.setOutlineThickness(2);

        sf::RectangleShape rect2;
        rect2.setPosition(200, 800);
        rect2.setSize(sf::Vector2f(200, 100));
        if (is_rb)
        {
            rect2.setFillColor(sf::Color::Yellow);
        }
        else
        {
            rect2.setFillColor(sf::Color::Black);
        }
        rect2.setOutlineColor(sf::Color::Red);
        rect2.setOutlineThickness(2);

        sf::RectangleShape rect3;
        rect3.setPosition(400, 800);
        rect3.setSize(sf::Vector2f(200, 100));
        if (is_dd)
        {
            rect3.setFillColor(sf::Color::Yellow);
        }
        else
        {
            rect3.setFillColor(sf::Color::Black);
        }
        rect3.setOutlineColor(sf::Color::Red);
        rect3.setOutlineThickness(2);

        sf::RectangleShape rect4;
        rect4.setPosition(600, 800);
        rect4.setSize(sf::Vector2f(200, 100));
        if (is_splay)
        {
            rect4.setFillColor(sf::Color::Yellow);
        }
        else
        {
            rect4.setFillColor(sf::Color::Black);
        }
        rect4.setOutlineColor(sf::Color::Red);
        rect4.setOutlineThickness(2);

        font.loadFromFile("C:/WINDOWS/Fonts/arial.ttf");

        sf::Text text1;
        text1.setFont(font);
        text1.setString("AVL");
        text1.setCharacterSize(70);
        text1.setFillColor(sf::Color::White);
        text1.setPosition(sf::Vector2f(40, 800));

        sf::Text text2;
        text2.setFont(font);
        text2.setString("RBT");
        text2.setCharacterSize(70);
        text2.setFillColor(sf::Color::White);
        text2.setPosition(sf::Vector2f(240, 800));

        sf::Text text3;
        text3.setFont(font);
        text3.setString("CT");
        text3.setCharacterSize(70);
        text3.setFillColor(sf::Color::White);
        text3.setPosition(sf::Vector2f(440, 800));

        sf::Text text4;
        text4.setFont(font);
        text4.setString("Splay");
        text4.setCharacterSize(70);
        text4.setFillColor(sf::Color::White);
        text4.setPosition(sf::Vector2f(610, 800));

        if (current_tree == "AVL")
        {
            rect1.setFillColor(sf::Color::Green);
            draw_avl(AVL_t, 0, 0, window, 0, 0);
        }
        else if (current_tree == "RB") 
        {
            rect2.setFillColor(sf::Color::Green);
            draw_rb(RB_t, 0, 0, window, 0, 0);
        }
        else if (current_tree == "DD")
        {
            rect3.setFillColor(sf::Color::Green);
            draw_dd(DD_t, 0, 0, window, 0, 0);
        }
        else if (current_tree == "SPLAY")
        {
            rect4.setFillColor(sf::Color::Green);
            draw_splay(Splay_t, 0, 0, window, 0, 0);
        }
        
        sf::RectangleShape val_insert;
        val_insert.setPosition(1000, 150);
        val_insert.setSize(sf::Vector2f(480, 100));
        if (ins_val) {
            //val_insert.setFillColor(sf::Color::Yellow);
            add_one_vertex();
            ins_val = false;
        }
        else val_insert.setFillColor(sf::Color::Black);

        sf::Text text_val_insert;
        text_val_insert.setFont(font);
        text_val_insert.setString("Enter the vertex key");
        text_val_insert.setCharacterSize(40);
        text_val_insert.setFillColor(sf::Color::White);
        text_val_insert.setPosition(sf::Vector2f(1030, 175));

        sf::RectangleShape rand_insert;
        rand_insert.setPosition(1000, 400);
        rand_insert.setSize(sf::Vector2f(480, 100));
        if (ins_rand) {
            //rand_insert.setFillColor(sf::Color::Yellow);
            add_lot_vertex();
            ins_rand = false;
        }
        else rand_insert.setFillColor(sf::Color::Black);

        sf::Text text_rand_insert;
        text_rand_insert.setFont(font);
        text_rand_insert.setString("Enter the number of\ngenerated vertices");
        text_rand_insert.setCharacterSize(40);
        text_rand_insert.setFillColor(sf::Color::White);
        text_rand_insert.setPosition(sf::Vector2f(1030, 400));

        window.draw(rect1);
        window.draw(rect2);
        window.draw(rect3);
        window.draw(rect4);
        window.draw(text1);
        window.draw(text2);
        window.draw(text3);
        window.draw(text4);

        window.draw(menu);
        window.draw(val_insert);
        window.draw(text_val_insert);
        window.draw(rand_insert);
        window.draw(text_rand_insert);
        window.display();
    }
    return 0;
}