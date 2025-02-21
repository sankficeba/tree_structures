#pragma once

class AVL
{
    struct Node;
    Node* t = nullptr;
public:
    void insert(int key);
    void remove(int key);
    void out();
    int num_cnt(Node* p);
private:
    int height(Node* p);
    int bfactor(Node* p);
    void fixheight(Node* p);
    Node* rotate_right(Node* p);
    Node* rotate_left(Node* p);
    Node* balance(Node* p);
    Node* find_min(Node* p);
    Node* remove_min(Node* p);
    Node* insert(Node* p, int key);
    Node* remove(Node* p, int key);
    void out(Node* t);
};