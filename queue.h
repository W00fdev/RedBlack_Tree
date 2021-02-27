#ifndef LAB_1_QUEUE_H
#define LAB_1_QUEUE_H

#include "node.h"
#include <iostream>

template <typename Key, typename Type>
class Queue {
private:
    size_t Size;
    Wrapper<Key, Type>* Head;
public:
    Queue() : Size(0), Head(nullptr)  {}

    void Enqueue(Node<Key, Type>* node_2) {
        if (Head == nullptr) {
            Head = new Wrapper(node_2);
        } else {
            Wrapper<Key, Type>* searcher = Head;
            while (searcher->wNext != nullptr) {
                searcher = searcher->wNext;
            }
            searcher->wNext = new Wrapper(node_2);
        }
        Size++;
    }

    Node<Key, Type>* Dequeue() {
        Node<Key, Type>* Data = Head->Core;
        PopFront();
        return Data;
    }

    Node<Key, Type>* Peek() {
        return Head->Core;
    }

    void PopFront() {
        Wrapper<Key, Type>* elem = Head;
        Head = Head->wNext;
        delete elem;
        Size--;
    }

    void Clear() {
        while (Size > 0)
            PopFront();
    }

    size_t GetSize() {
        return Size;
    }

    bool IsEmpty() {
        return Size == 0;
    }
};


#endif //LAB_1_QUEUE_H