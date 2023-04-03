#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    void right_rotate(AVLNode<Key, Value>* loc_);

    void left_rotate(AVLNode<Key, Value>* loc_);

    void insertRec( AVLNode<Key,Value>* p, const std::pair<const Key, Value>& ins);
    void refreshBalance(AVLNode<Key, Value>* curr);

    void removeRec(AVLNode<Key,Value>* curr, const Key& key);

};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    if (this->root_ == nullptr) {
        this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
    }
    else {
        insertRec(static_cast<AVLNode<Key, Value>*>(this->root_), new_item);
    }
}

template <class Key, class Value>
void AVLTree<Key, Value>::insertRec(AVLNode<Key, Value>* curr, const std::pair<const Key, Value>& ins) {

    if (curr->getKey() < ins.first) {
        if (curr->getRight() == nullptr) {
            curr->setRight(new AVLNode<Key, Value>(ins.first, ins.second, curr));
        }
        else {
            insertRec(curr->getRight(), ins);
        }
    }
    else if (curr->getKey() > ins.first) {
        if (curr->getLeft() == nullptr) {
            curr->setLeft(new AVLNode<Key, Value>(ins.first, ins.second, curr));
        }
        else {
            insertRec(curr->getLeft(), ins);
        }
    }
    else {
        curr->setValue(ins.second);
    }

    refreshBalance(curr);

    if (curr->getBalance() == 2) {
        AVLNode<Key, Value>* child = curr->getRight();
        if (child->getBalance()  == -1) {
            right_rotate(child);
        }
        left_rotate(curr);
    }
    else if (curr->getBalance()  == -2) {
        AVLNode<Key, Value>* child = curr->getLeft();
        if(child->getBalance() == 1) {
            left_rotate(child);
        }
        right_rotate(curr);
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::refreshBalance(AVLNode<Key, Value>* curr) {
    if (curr == nullptr) return;

    int rightHeight = BinarySearchTree<Key, Value>::degree(curr->getRight());
    int leftHeight = BinarySearchTree<Key, Value>::degree(curr->getLeft());

    curr->setBalance(rightHeight - leftHeight);
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


template<class Key, class Value>
void AVLTree<Key, Value>::right_rotate(AVLNode<Key, Value>* loc_)
{
//TODO
    AVLNode<Key,Value>* left_child= loc_->getLeft();
    AVLNode<Key, Value>* jelly = left_child->getRight();
    if(this->root_ == loc_){
        this->root_ = left_child;
        left_child->setParent(nullptr);
    }

    else{

    AVLNode<Key,Value>* gp = loc_ -> getParent();
		left_child -> setParent(gp);
		if (gp->getRight() == loc_) gp->setRight(left_child);
		else if (gp->getLeft() == loc_) gp->setLeft(left_child);
    }
	loc_->setLeft(jelly);
	if (jelly != nullptr) jelly->setParent(loc_);

    loc_ -> setParent(left_child);
	left_child -> setRight(loc_);

}

template<class Key, class Value>
void AVLTree<Key, Value>::left_rotate(AVLNode<Key, Value>* loc_)
{
//TODO

    AVLNode<Key,Value>* left_child= loc_->getRight();
    AVLNode<Key, Value>* jelly = left_child->getLeft();
    if(this->root_ == loc_){
        this->root_ = left_child;
        left_child->setParent(nullptr);
    }
    else{
        AVLNode<Key,Value>* gp = loc_ -> getParent();
        left_child -> setParent(gp);
        if (gp->getLeft() == loc_) gp->setLeft(left_child);
        else if (gp->getRight() == loc_) gp->setRight(left_child);
    }

	loc_->setRight(jelly);
	if (jelly != nullptr) jelly->setParent(loc_);

    loc_ -> setParent(left_child);
	left_child -> setLeft(loc_);

}

template<typename Key, typename Value>
void AVLTree<Key, Value>::remove(const Key& key) {
    removeRec(static_cast<AVLNode<Key, Value>*> (this->root_), key);
}

template<typename Key, typename Value>
void AVLTree<Key, Value>::removeRec(AVLNode<Key,Value>* curr, const Key& key) 
{
    if (curr == nullptr) return; 

    Key currKey = curr->getKey();
    if (currKey < key) {
        removeRec(curr->getRight(), key);
    }
    else if (currKey > key) {
        removeRec(curr->getLeft(), key);
    }
    else {
        // For no children remove
        if (curr->getLeft() == nullptr && curr->getRight() == nullptr) {
            AVLNode<Key, Value>* parent = curr->getParent();
            if (parent == nullptr) {
                this->root_ = nullptr;
            }
            else {
                if (parent->getRight() == curr) {
                    parent->setRight(nullptr);
                }
                else {
                    parent->setLeft(nullptr);
                }
            }
            delete curr; 
            return;
        }

        else if (curr->getLeft() != nullptr && curr->getRight() == nullptr) {
            AVLNode<Key, Value>* parent = curr->getParent();
            if (parent == nullptr) {
                this->root_ = curr->getLeft();
                curr->getLeft()->setParent(nullptr);
            }
            else {
                if (parent->getRight() == curr) {
                    parent->setRight(curr->getLeft());
                }
                else {
                    parent->setLeft(curr->getLeft());
                }
                curr->getLeft()->setParent(parent);
            }

            delete curr;
            return;
        }

        else if (curr->getLeft() == nullptr && curr->getRight() != nullptr) {
            AVLNode<Key, Value>* parent = curr->getParent();
            if (parent == nullptr) {
                this->root_ = curr->getRight();
                curr->getRight()->setParent(nullptr);
            }
            else {
                if (parent->getRight() == curr) {
                    parent->setRight(curr->getRight());
                }
                else {
                    parent->setLeft(curr->getRight());
                }
                curr->getRight()->setParent(parent);
            }

            delete curr;
            return;
        }

        else {
            AVLNode<Key, Value>* swapper = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::predecessor(curr));
            nodeSwap(curr, swapper);
            removeRec(curr, key);
            return;
        }
    }

    refreshBalance(curr);

    if (curr->getBalance() == 2) {
        AVLNode<Key, Value>* child = curr->getRight();
        if (child->getBalance()  == -1) {
            right_rotate(child);
        }
        left_rotate(curr);
    }
    else if (curr->getBalance()  == -2) {
        AVLNode<Key, Value>* child = curr->getLeft();
        if(child->getBalance() == 1) {
            left_rotate(child);
        }
        right_rotate(curr);
    }
}

#endif
