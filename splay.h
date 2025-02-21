#pragma once

class Splay
{
    struct Node;
    Node* t = nullptr;
public:
    void insert(int key);
    void remove(int key);
private:
    struct Node;
    Node* left_zig(Node*& x);
    Node* right_zig(Node*& x);
    Node* left_zig_zig(Node*& x);
    Node* right_zig_zig(Node*& x);
    Node* left_zig_zag(Node*& x);
    Node* right_zig_zag(Node*& x);
    Node* splay(Node* p);
    Node* insert(Node*& p, int key, Node* parent);
    Node* max(Node* p);
    Node* remove(Node*& p, int key);
};