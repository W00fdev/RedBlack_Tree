#ifndef LAB_1_NODE_H
#define LAB_1_NODE_H

#define RED false
#define BLACK true

template <typename Key, typename Type>
class Node{
public:
    Key key;
    Type value;
    Node* parent;
    Node* left;
    Node* right;

    bool color = RED;
    bool isLeaf = false;


    Node(const Key& new_key, const Type& new_value, Node* new_parent = nullptr, Node* new_left = nullptr, Node* new_right = nullptr) {
        key = new_key;
        value = new_value;
        parent = new_parent;
        left = new_left;
        right = new_right;
    }

    Node() {
        parent = nullptr;
        left = nullptr;
        right = nullptr;
        color = BLACK;
        isLeaf = true;
    }

};

template<typename Key, typename Type>
class Wrapper {
public:
    Node<Key, Type>* Core;
    Wrapper* wNext;

    Wrapper(Node<Key, Type>* core_2, Wrapper* wNext_2 = nullptr) : Core(core_2), wNext(wNext_2) {}
    Wrapper() : Core(nullptr), wNext(nullptr) {}
};

#endif //LAB_1_NODE_H
