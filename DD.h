#pragma once

class DD
{
    struct Node;
    Node* t = nullptr;
public:
    void insert(int key);
    void remove(int key);
private:
    int height(Node* t);
    void update_height(Node* t);
    void merge(Node*& t, Node* first_t, Node* second_t);
    void split(Node* t, Node*& l, Node*& r, int key, int add);
    void insert(Node*& t, int key);
    void remove(Node*& t, int key);
    void output(Node* t);
};