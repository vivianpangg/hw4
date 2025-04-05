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
    void insertFix(AVLNode<Key,Value>* parentNode,AVLNode<Key,Value>* current);
    int getNodeBalance(AVLNode<Key,Value>* node);
    // void clearNodes(AVLNode<Key,Value>* node);
    void rotateRight(AVLNode<Key,Value>* parentNode);
    void rotateLeft(AVLNode<Key,Value>* parentNode);
    void removeFix(AVLNode<Key, Value>* node , int diff);


};

template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value>*parent, AVLNode<Key, Value>* current){
    if(parent ==nullptr || parent->getParent()==nullptr){
        return;

    }

    AVLNode<Key,Value>* grandparent =parent->getParent();
    if(parent==grandparent->getLeft()){
        grandparent->updateBalance(-1);
        if(grandparent->getBalance()==0){
            return;
        }else if(grandparent->getBalance()==-1){
            insertFix(grandparent,parent);
        }else if(grandparent->getBalance()==-2){
            if(parent->getBalance()==-1){
                rotateRight(grandparent);
                parent->setBalance(0);
                grandparent->setBalance(0);
            }else if(parent->getBalance()==1){
                AVLNode<Key,Value>* child = parent->getRight();
                rotateLeft(parent);
                rotateRight(grandparent);
                

                if(child->getBalance() ==-1){
                    parent->setBalance(0);
                    grandparent->setBalance(1);
                    child->setBalance(0);

                }else if(child->getBalance() ==0){
                    parent->setBalance(0);
                    grandparent->setBalance(0);
                    child->setBalance(0);

                }else if(child->getBalance() ==1){
                    parent->setBalance(-1);
                    grandparent->setBalance(0);
                    child->setBalance(0);

                }

            }

        }

    }else{
        grandparent->updateBalance(1);

        if(grandparent->getBalance()==0){
            return;
        }else if(grandparent->getBalance()==1){
            insertFix(grandparent,parent);
        }else if(grandparent->getBalance()==2){
            if(parent->getBalance()==1){
                rotateLeft(grandparent);
                parent->setBalance(0);
                grandparent->setBalance(0);

            }else if(parent->getBalance()==-1){
                AVLNode<Key,Value>* child = parent->getLeft();
                rotateRight(parent);
                rotateLeft(grandparent);


                if(child->getBalance() ==1){
                    parent->setBalance(0);
                    grandparent->setBalance(-1);
                    child->setBalance(0);

                }else if(child->getBalance() ==0){
                    parent->setBalance(0);
                    grandparent->setBalance(0);
                    child->setBalance(0);

                }else if(child->getBalance() ==-1){
                    parent->setBalance(1);
                    grandparent->setBalance(0);
                    child->setBalance(0);

                }
                

            }

        }
    }
}


template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* node){
    AVLNode<Key, Value>* leftC = node->getLeft();
    node->setLeft(leftC->getRight());

    if(leftC->getRight()!=nullptr){
        leftC->getRight()->setParent(node);
    }


    leftC->setParent(node->getParent());
    if(node->getParent()==nullptr){
        this->root_=leftC;

    }else if(node ==node->getParent()->getLeft()){
        node->getParent()->setLeft(leftC);

    }else{
        node->getParent()->setRight(leftC);

    }

    leftC->setRight(node);
    node->setParent(leftC);

}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* node){
    AVLNode<Key, Value>* rightC = node->getRight();
    node->setRight(rightC->getLeft());

    if(rightC->getLeft()!=nullptr){
        rightC->getLeft()->setParent(node);
    }


    rightC->setParent(node->getParent());
    if(node->getParent()==nullptr){
        this->root_=rightC;

    }else if(node ==node->getParent()->getLeft()){
        node->getParent()->setLeft(rightC);

    }else{
        node->getParent()->setRight(rightC);

    }

    rightC->setLeft(node);
    node->setParent(rightC);

}
/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */

template<class Key, class Value>
void AVLTree<Key, Value>::remove(const Key& key){
    AVLNode<Key, Value>* node = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));

    if(node ==nullptr){
        return;

    }


    if(node->getLeft()!= nullptr&&node->getRight()!=nullptr){
        AVLNode<Key, Value>* before = static_cast<AVLNode<Key, Value>*>(this->predecessor(node));
        nodeSwap(node, before);
    }
     AVLNode<Key, Value>* parent = node->getParent();
     AVLNode<Key, Value>* child = (node->getLeft() !=nullptr)?node->getLeft():node->getRight();


     int difference = 0;
     if(parent != nullptr){
        if(node ==parent->getLeft()){
            difference =1;
        }else{
            difference = -1;
        }

     }


     if(child!= nullptr){
        child->setParent(parent);

     }

     if(parent == nullptr){
        this->root_ =child;
     }else{
        if(node==parent->getLeft()){
            parent->setLeft(child);
        }else{
            parent->setRight(child);

        }
     }

     delete node;

     if(parent!= nullptr){
        removeFix(parent,difference);

     }

    
}

template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value>* node, int diff){
    if(node ==nullptr){
        return;

    }
    AVLNode<Key, Value>* parent = node->getParent();
    int nextD = 0;
    if(parent!= nullptr){
        if(node==parent->getLeft()){
            nextD=1;

        }else{
            nextD=-1;
        }

    }

    if(diff == -1){
        node->updateBalance(diff);

        if(node->getBalance()==-2){
            AVLNode<Key, Value>* leftC = node->getLeft();

            if(leftC->getBalance()<=0){
                rotateRight(node);
                if(leftC->getBalance()==0){
                    node->setBalance(-1);
                    leftC->setBalance(1);
                    return;

                }else{
                    node->setBalance(0);
                    leftC->setBalance(0);
                }
            }else{
                AVLNode<Key, Value>* grandC = leftC->getRight();
                rotateLeft(leftC);
                rotateRight(node);

                if(grandC->getBalance()==-1){
                    node->setBalance(1);
                    leftC->setBalance(0);
                    grandC->setBalance(0);

                }else if(grandC->getBalance()==0){
                    node->setBalance(0);
                    leftC->setBalance(0);
                    grandC->setBalance(0);

                }else if(grandC->getBalance()==1){
                    node->setBalance(0);
                    leftC->setBalance(-1);
                    grandC->setBalance(0);

                }
            }

        }else if(node->getBalance()==-1){
            return;

        }
    }else if(diff ==1){
        node->updateBalance(diff);

        if(node->getBalance()==2){
            AVLNode<Key,Value>* rightC = node->getRight();

            if(rightC->getBalance()>=0){
                rotateLeft(node);

                if(rightC->getBalance()==0){
                    node->setBalance(1);
                    rightC->setBalance(-1);
                    return;

                }else{
                    node->setBalance(0);
                    rightC->setBalance(0);
                }

            }else{
                AVLNode<Key, Value>* grandC = rightC->getLeft();
                rotateRight(rightC);
                rotateLeft(node);

                if(grandC->getBalance()==1){
                    node->setBalance(-1);
                    rightC->setBalance(0);
                    grandC->setBalance(0);

                }else if(grandC->getBalance()==0){
                    node->setBalance(0);
                    rightC->setBalance(0);
                    grandC->setBalance(0);

                }else if(grandC->getBalance()==-1){
                    node->setBalance(0);
                    rightC->setBalance(1);
                    grandC->setBalance(0);

                }
            }

        }else if(node->getBalance()==1){
            return;

        }
    }

    if(parent!= nullptr){
        removeFix(parent, nextD);

    }

}

template<class Key, class Value>
int AVLTree<Key,Value>::getNodeBalance(AVLNode<Key, Value>* node){
    if(node==nullptr){
        return 0;

    }

    return node->getBalance();
}


/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    if(this->root_==nullptr){
        this->root_ = new AVLNode<Key,Value>(new_item.first,new_item.second,nullptr);
        return;

    }

    AVLNode<Key,Value>* current = static_cast<AVLNode<Key,Value>*>(this->root_);
    AVLNode<Key,Value>* parent = nullptr;

    while(current != nullptr){
        parent = current;

        if(new_item.first<current->getKey()){
            current = current->getLeft();

        }else if(new_item.first>current->getKey()){
            current=current->getRight();
        }else{
            current->setValue(new_item.second);
            return;

        }
    }
    AVLNode<Key,Value>* newNode = new AVLNode<Key,Value>(new_item.first,new_item.second,parent);

    if(new_item.first<parent->getKey()){
        parent->setLeft(newNode);
    }else{
        parent->setRight(newNode);
    }

    if(parent->getBalance()==-1||parent->getBalance()==1){
        parent->setBalance(0);
    }else{
        if(newNode==parent->getLeft()){
            parent->updateBalance(-1);
        }else{
            parent->updateBalance(1);
        }

        if(parent->getBalance()!=0){
            insertFix(parent,newNode);

        }
    }



}


template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
