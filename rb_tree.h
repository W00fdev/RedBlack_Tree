#ifndef LAB_1_RB_TREE_H
#define LAB_1_RB_TREE_H

//#define RED false
//#define BLACK true

#include <vector>
#include "queue.h"


template<typename Key, typename Type>
class RB_map {
private:
    Node<Key, Type>* root = nullptr;

    // easy leaf creating
    Node<Key, Type>* create_leaf(Node<Key, Type>* node_before, bool left) {
        Node<Key, Type>* leaf = new Node<Key, Type>();    // default constructor creates leaves
        if (left) {
            node_before->left = leaf;
        } else {
            node_before->right = leaf;
        }

        leaf->parent = node_before;
    }

    // easy "inline" check
    bool has_2_leaf(Node<Key, Type>* node_for_check) {
        if (node_for_check == nullptr) throw std::invalid_argument("Node for check doesn't exist");
        if (node_for_check->isLeaf == false) {
            return false;
        }
        return node_for_check->left->isLeaf && node_for_check->right->isLeaf;
    }

    // needs for removing
    Node<Key, Type>* find_the_biggest(Node<Key, Type>* start_node) {
        Node<Key, Type>* runner = start_node;
        while (runner->right != nullptr && runner->right->isLeaf != true) {
            runner = runner->right;
        }

        if (runner == nullptr || runner->isLeaf) throw std::runtime_error("The biggest find alg error");
        return runner;
    }

    /*
     *      ALL PRIVATE FUNCS THAT MAINTAIN RB_TREE
     */

    // [delete additional]
    void replace_node(Node<Key, Type>* node, Node<Key, Type>* child) {
        child->parent = node->parent;
        if (node == node->parent->left)
            node->parent->left = child;
        else
            node->parent->right = child;
    }

    // [Delete additional]
    void delete_one_child(Node<Key, Type>* node) {
        /*
         *  n must has only 1 non-leaf child
         */
        Node<Key, Type>* child;
        Node<Key, Type>* leaf_to_delete;

        if (node->left->isLeaf) {
            child = node->right;
            leaf_to_delete = node->left;
        }
        else {
            child = node->left;
            leaf_to_delete = node->right;
        }

        replace_node(node, child);
        if (node->color == BLACK) {        // NODE == BLACK (if node is RED just delete it)
            if (child->color == RED) {
                child->color = BLACK;      // now CHILD IS BLACK and on position of the node
            } else
                delete_case1(child);
        }
        delete leaf_to_delete;
        delete node;
    }

    // [Delete cases]
    // if node is root then tree is valid
    void delete_case1(Node<Key, Type>* node) {
        if (node->parent != nullptr)
            delete_case2(node);
        else
            node->color = BLACK;
    }

    // rotate relatively parent if brother is red
    void delete_case2(Node<Key, Type>* node) {
        Node<Key, Type>* _sibling = siblings(node);

        if (_sibling->color == RED) {
            node->parent->color = RED;
            _sibling->color = BLACK;
            if (node == node->parent->left)
                rotate_left(node->parent);
            else
                rotate_right(node->parent);
        }
        delete_case3(node);
    }

    // brother's color case
    void delete_case3(Node<Key, Type>* node) {
        Node<Key, Type> *_sibling = siblings(node);

        if (node->parent->color == BLACK) {
            if (_sibling->color == BLACK) {
                if (_sibling->left->color == _sibling->right->color == BLACK) {  // if brother's children are black
                    _sibling->color = RED;  // now BROTHER is RED
                    delete_case1(node->parent);
                    return;
                }
            }
        }
        delete_case4(node);
    }

    // brother's color case
    void delete_case4(Node<Key, Type>* node) {
        Node<Key, Type> *_sibling = siblings(node);

        if (node->parent->color == RED) {
            if (_sibling->color == BLACK) {
                if (_sibling->left->color == _sibling->right->color == BLACK) {  // if brother's children are black
                    _sibling->color = RED;
                    node->parent->color = BLACK;
                    return;
                }
            }
        }
        delete_case5(node);
    }

    // pre-rotate before final rotation
    void delete_case5(Node<Key, Type>* node)
    {
        Node<Key, Type>* _sibling = siblings(node);

        if  (_sibling->color == BLACK) {
/* the statements just force the red to be on the left of the left of the parent,
   or right of the right, so case six will rotate correctly. */
            if ((node == node->parent->left) &&
                (_sibling->right->color == BLACK) &&
                (_sibling->left->color == RED)) {
                _sibling->color = RED;
                _sibling->left->color = BLACK;
                rotate_right(_sibling);
            } else if ((node == node->parent->right) &&
                       (_sibling->left->color == BLACK) &&
                       (_sibling->right->color == RED)) {
                _sibling->color = RED;
                _sibling->right->color = BLACK;
                rotate_left(_sibling);
            }
        }
        delete_case6(node);
    }

    // rotation cases
    void delete_case6(Node<Key, Type>* node)
    {
        Node<Key, Type>* _sibling = siblings(node);

        _sibling->color = node->parent->color;
        node->parent->color = BLACK;

        if (node == node->parent->left) {
            _sibling->right->color = BLACK;
            rotate_left(node->parent);
        } else {
            _sibling->left->color = BLACK;
            rotate_right(node->parent);
        }
    }
    // [Delete cases]

    // [Find the family nodes]
    Node<Key, Type>* grandparent(Node<Key, Type>* node) {
        if(node != nullptr && node->parent != nullptr)
            return node->parent->parent;

        return nullptr; // there's no grandpa
    }

    Node<Key, Type>* uncle(Node<Key, Type>* node) {
        Node<Key, Type>* grandpa = grandparent(node);
        if (grandpa == nullptr) {
            return nullptr; // if there's no grandpa then there's no uncle
        }
        if (node->parent == grandpa->left) return grandpa->right;
        else return grandpa->left;
    }

    Node<Key, Type>* siblings(Node<Key, Type>* node) {
        if (node == node->parent->left)
            return node->parent->right;
        else
            return node->parent->left;
    }
    // [Find the family nodes]

    // [Rotates]
    void rotate_left(Node<Key, Type>* node) {
        Node<Key, Type>* pivot = node->right;

        pivot->parent = node->parent;   // maybe pivot now is the root
        if (node->parent != nullptr) {
            if (node->parent->left == node) // if node was the left/right child of the parent
                node->parent->left = pivot; // now pivot is the child of the node's parent
            else
                node->parent->right = pivot;
        } else
            root = pivot;   // new root of the tree

        node->right = pivot->left;  // we take to the right pivot's left child
        if (pivot->left != nullptr) { // if pivot's left child is not leaf
            pivot->left->parent = node; // link him to the node
        }
        node->parent = pivot;
        pivot->left = node; // we've totally changed pivot and node
    }

    void rotate_right(Node<Key, Type>* node) {
        // the same as "rotate_left" but in other direction
        Node<Key, Type>* pivot = node->left;

        pivot->parent = node->parent;   // maybe pivot now is the root
        if (node->parent != nullptr) {
            if (node->parent->left == node)
                node->parent->left = pivot;
            else
                node->parent->right = pivot;
        }
        else
            root = pivot;

        node->left = pivot->right;
        if (pivot->right != nullptr) {
            pivot->right->parent = node;
        }
        node->parent = pivot;
        pivot->right = node; // we've totally changed pivot and node
    }
    // [Rotates]

    // [Insert Cases]

    // if node is the root we change its color to black
    void insert_case1(Node<Key, Type>* node) {
        if (node->parent == nullptr) {
            node->color = BLACK;
        }
        else insert_case2(node);
    }

    // check the father's color
    void insert_case2(Node<Key, Type>* node) {
        if (node->parent->color == BLACK) {
            // node == RED => hb = const still.
            return;     // tree is valid
        }
        else insert_case3(node);
    }

    // if the Uncle and the Parent are red.
    // by the insert_case2 node->parent == RED => node has the black grandpa
    void insert_case3(Node<Key, Type>* node) {
        Node<Key, Type>* _uncle = uncle(node);
        Node<Key, Type>* grandpa = grandparent(node);

        if (_uncle != nullptr /*&& _uncle->isLeaf != true */ && _uncle->color == RED) {
            // parent is already red

            node->parent->color = BLACK;   // recolor to black P and U
            _uncle->color = BLACK;

            grandpa->color = RED;       // grandpa now is red
            insert_case1(grandpa);  // repeat case 1 (if grandpa is root)
        } else {
            insert_case4(node);
        }
    }

    // uncle in case3 wasn't red, but black
    // then the tree has 2 red nodes (parent -> node)
    // so we can rotate the tree
    void insert_case4(Node<Key, Type>* node) {
        Node<Key, Type>* grandpa = grandparent(node);

        // find the location of node and parent (to the right or to the left)
        // rotate direction depends on it
        if ((node == node->parent->right) && (node->parent == grandpa->left) ) {
            rotate_left(node->parent);
            node = node->left;
        } else if ((node == node->parent->left) && (node->parent == grandpa->right) ) {
            rotate_right(node->parent);
            node = node->right;
        }
        // after direction correction (or it's already correct) run case5
        insert_case5(node);
    }

    void insert_case5(Node<Key, Type>* node) {
        Node<Key, Type>* grandpa = grandparent(node);

        node->parent->color = BLACK;
        grandpa->color = RED;

        // find the location of node and parent (to the right or to the left)
        // rotate direction depends on it
        if ((node == node->parent->left) && (node->parent == grandpa->left)) {
            rotate_right(grandpa);
        } else if ((node == node->parent->right) && (node->parent == grandpa->right)) {
            rotate_left(grandpa);
        }
    }


    // [Insert Cases]

    void clear_tree(Node<Key, Type>* node_to_clear) {
        if (node_to_clear != nullptr) {
            clear_tree(node_to_clear->left);
            node_to_clear->left = nullptr;
            clear_tree(node_to_clear->right);
            node_to_clear->right = nullptr;
            delete node_to_clear;
        }
    }

public:
    typedef Node<Key, Type>* PNode;

    ~RB_map() {
        clear_tree(root);
        root = nullptr;
    }

    void clear() {
        clear_tree(root);
        root = nullptr;
    }

    // bfs

    void print() {
        if (root == nullptr) return;
        Queue<Key, Type> QueueList;
        QueueList.Enqueue(root);

        while (QueueList.GetSize() > 0) {
            Node<Key, Type> *current_node = QueueList.Dequeue();

            if (current_node->isLeaf) std::cout << "leaf ";
            else {
                std::cout << current_node->key << ":" << current_node->value;
                if (current_node->color == BLACK) std::cout << "b ";
                else std::cout << "r ";
            }

            if (current_node->left != nullptr) QueueList.Enqueue(current_node->left);
            if (current_node->right != nullptr) QueueList.Enqueue(current_node->right);
        }
    }

    std::vector<Key> get_keys() {
        std::vector<Key> vec_keys;

        if (root == nullptr) throw std::runtime_error("Can't get keys from empty map");
        Queue<Key, Type> QueueList;
        QueueList.Enqueue(root);

        while (QueueList.GetSize() > 0) {
            Node<Key, Type> *current_node = QueueList.Dequeue();
            if (current_node->isLeaf == false)
                vec_keys.emplace_back(current_node->key);

            if (current_node->left != nullptr) QueueList.Enqueue(current_node->left);
            if (current_node->right != nullptr) QueueList.Enqueue(current_node->right);
        }
        return vec_keys;
    }

    std::vector<Type> get_values() {
        std::vector<Type> vec_values;

        if (root == nullptr) throw std::runtime_error("Can't get values from empty map");

        Queue<Key, Type> QueueList;
        QueueList.Enqueue(root);

        while (QueueList.GetSize() > 0) {
            Node<Key, Type> *current_node = QueueList.Dequeue();
            if (current_node->isLeaf == false)
                vec_values.emplace_back(current_node->value);

            if (current_node->left != nullptr) QueueList.Enqueue(current_node->left);
            if (current_node->right != nullptr) QueueList.Enqueue(current_node->right);
        }
        return vec_values;
    }

    Node<Key, Type>* find(const Key& find_key) {
        Node<Key, Type>* runner = root;
        if (root == nullptr) throw std::runtime_error("Tree is empty");

        while(runner != nullptr && runner->isLeaf == false) {
            if (find_key == runner->key) {
                if (!runner->left->isLeaf) {
                    if (runner->left->key == find_key) {
                        runner = runner->left;
                        continue;
                    }
                } else if (!runner->right->isLeaf) {
                    if (runner->right->key == find_key) {
                        runner = runner->right;
                        continue;
                    }
                }
                break;  // algorithm has found right element
            } else if (find_key > runner->key) {
                if (runner->right != nullptr && runner->right->isLeaf == false) runner = runner->right;
                else break;
            } else {
                if (runner->left != nullptr && runner->left->isLeaf == false) runner = runner->left;
                else break;
            }
        }
        if (runner->key != find_key) throw std::runtime_error("Can't find key in find()");
        return runner;
    }

    void insert(const Key& new_key, const Type& new_value) {
        if (root == nullptr) {
            root = new Node<Key, Type>(new_key, new_value);
            insert_case1(root);           // check insert cases
            create_leaf(root, true);    // left_leaf create
            create_leaf(root, false);   // right_leaf create

            return;
        }

        Node<Key, Type>* runner = root;
        while(true) {
            if (new_key >= runner->key) {
                if (runner->right != nullptr && runner->right->isLeaf == false) runner = runner->right;
                else {
                    Node<Key, Type> *leaf_to_move = runner->right;   // supposed to be initialized leaf
                    runner->right = new Node<Key, Type>(new_key, new_value, runner);
                    runner->right->right = leaf_to_move;
                    leaf_to_move->parent = runner->right;
                    if (runner->right->left == nullptr)
                        create_leaf(runner->right, true);   // create left leaf

                    insert_case1(runner->right);
                    insert_case1(root);
                    break;
                }
            } else{
                if (runner->left != nullptr && runner->left->isLeaf == false) runner = runner->left;
                else {
                    Node<Key, Type>* leaf_to_move = runner->left;   // supposed to be initialized leaf
                    runner->left = new Node<Key, Type>(new_key, new_value, runner);
                    runner->left->left = leaf_to_move;
                    leaf_to_move->parent = runner->left;
                    if (runner->left->right == nullptr)
                        create_leaf(runner->left, false);   // create right leaf
                    insert_case1(runner->left);
                    insert_case1(root);
                    break;
                }
            }
        }
    }

    void remove(const Type& key_to_delete) {
        Node<Key, Type>* runner = root;
        while (runner != nullptr && runner->isLeaf != true) {
            if (runner->key == key_to_delete) {                 // finds the last equal node
                if (runner->left != nullptr && !runner->left->isLeaf) {
                    if (runner->left->key == key_to_delete) {
                        runner = runner->left;
                        continue;
                    }
                } else if (runner->right != nullptr && !runner->right->isLeaf) {
                    if (runner->right->key == key_to_delete) {
                        runner = runner->right;
                        continue;
                    }
                }
                break;
            }
            if (key_to_delete >= runner->key) runner = runner->right;
            else runner = runner->left;
        }

        // if there's no error;  runner is node to delete
        if (runner != nullptr && runner->key == key_to_delete) {
            if (!runner->left->isLeaf && !runner->right->isLeaf) {  // if node has 2 children
                if (has_2_leaf(runner->left)) { // change them
                    runner->value = runner->left->value;    // change values
                    runner->left->isLeaf = true;            // make it leaf
                    runner->left->color = BLACK;

                    if (runner->left->left != nullptr) {
                        delete runner->left->left;
                        runner->left->left = nullptr;
                    }
                    if (runner->left->right != nullptr) {    // delete old leaves
                        delete runner->left->right;
                        runner->left->right = nullptr;
                    }

                    if (runner->left->color == BLACK) {
                        delete_case1(runner);   // in which color of runner make case
                    }
                } else {
                    // we need the biggest node in left subtree
                    Node<Key, Type>* to_change = find_the_biggest(runner->left);
                    to_change->isLeaf = true;
                    to_change->color = BLACK;

                    runner->value = to_change->value;
                    runner->key = to_change->key;

                    if (to_change->left != nullptr) {
                        delete to_change->left;
                        to_change->left = nullptr;
                    }
                    if (to_change->right != nullptr) {     // delete old leaves
                        delete to_change->right;
                        to_change->right = nullptr;
                    }

                    if (to_change->color == BLACK) {
                        delete_case1(to_change->parent);
                    }

                }
            } else if (has_2_leaf(runner)) { // if node hasn't children
                if (runner->left != nullptr) {
                    delete runner->left;
                    runner->left = nullptr;
                }
                if (runner->right != nullptr) {  // delete old leaves
                    delete runner->right;
                    runner->right = nullptr;
                }

                if (runner->parent == nullptr) {
                    delete runner;
                    root = nullptr;
                } else {
                    runner->isLeaf = true;
                    runner->color = BLACK;
                }

            } else {    // if node has 1 child
                delete_one_child(runner);
            }
        } else {
            throw std::invalid_argument("Can't find the node to delete");
        }
    }

};

#endif //LAB_1_RB_TREE_H
