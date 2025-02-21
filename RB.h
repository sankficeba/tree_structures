#pragma once

class RB
{
    struct Node;
    Node* t = nullptr;
public:
    void insert(int key);
private:
    int height(Node* p);
    void fixheight(Node*& p);
    void ins_balance(Node* n);
    void insert(Node*& p, int key, Node* pred);
};