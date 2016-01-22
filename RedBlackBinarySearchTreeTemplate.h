/*
 * rbbst.h
 *
 * Date        Author    Notes
 * =====================================================
 * 2014-04-14  Kempe     Initial version
 * 2015-04-06  Redekopp  Updated formatting and removed
 *                         KeyExistsException
 */
#ifndef RBBST_H
#define RBBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include "bst.h"

/* -----------------------------------------------------
 * Red-Black Nodes and Search Tree
 ------------------------------------------------------*/

enum Color {red, black};

template <class KeyType, class ValueType>
class RedBlackNode : public Node<KeyType, ValueType>
{
public:
  RedBlackNode (KeyType k, ValueType v, RedBlackNode<KeyType, ValueType> *p)
    : Node<KeyType, ValueType> (k, v, p)
    { color = red; }
  
  virtual ~RedBlackNode () {}
  
  Color getColor () const
    { return color; }
  
  void setColor (Color c)
    { color = c; }
  
  virtual RedBlackNode<KeyType, ValueType> *getParent () const
    { return (RedBlackNode<KeyType,ValueType>*) this->_parent; }
  
  virtual RedBlackNode<KeyType, ValueType> *getLeft () const
    { return (RedBlackNode<KeyType,ValueType>*) this->_left; }
  
  virtual RedBlackNode<KeyType, ValueType> *getRight () const
    { return (RedBlackNode<KeyType,ValueType>*) this->_right; }
  
 protected:
  Color color;

  // std::pair<const KeyType, ValueType> _item;
  // Node <KeyType, ValueType> *_left, *_right, *_parent;
};

/* -----------------------------------------------------
 * Red-Black Search Tree
 ------------------------------------------------------*/

template <class KeyType, class ValueType>
class RedBlackTree : public BinarySearchTree<KeyType, ValueType>
{
public:
  void insert (const std::pair<const KeyType, ValueType>& new_item){
  /* This one is yours to implement.
     It should insert the (key, value) pair to the tree, 
     making sure that it remains a valid Red-Black Tree.
     If the key is already in the tree
     (with a possibly different associated value),
     then it should overwrite the value with the new one.
     We strongly recommend defining suitable (private) helper functions. */
     RedBlackNode<KeyType, ValueType>* toBeInserted = new RedBlackNode<KeyType, ValueType>(new_item.first, new_item.second, NULL);

     if(this->root == NULL){
        this->root = toBeInserted;
     }
     else{    
        //first, find the place in the binary tree that the new value should be inserted, insert it
        RedBlackNode<KeyType, ValueType>* temp =  (RedBlackNode<KeyType, ValueType>*) this->root;
        while(true){
          //if the key exists, replace the node with new value
          if(toBeInserted->getKey() == temp->getKey()){
            temp->setValue(toBeInserted->getValue());
            delete toBeInserted;        //if the node already exist, only copy the node and free the memory
            return; 
          }
          //look at left node if the insertion value is less than the current node's value
          else if(toBeInserted->getKey() < temp->getKey()){
            if(temp->getLeft()!=NULL){
                temp = temp->getLeft();
            }
            else if(temp->getLeft()==NULL){
              temp->setLeft(toBeInserted);
              toBeInserted->setParent(temp);
              break;
            }
          }
          //look at right node if the insertion value is greater than the current node's value
          else if(toBeInserted->getKey() > temp->getKey()){
            if(temp->getRight()!=NULL){
                 temp = temp->getRight();
            }
            else if(temp->getRight()==NULL){
              temp->setRight(toBeInserted);
              toBeInserted->setParent(temp);
              break;
            }
          }
        }
        // then solve any violation to the deniftion of RBTrees
     }
     makeValid(toBeInserted); 
}
private:
  void makeValid(RedBlackNode<KeyType, ValueType>*  insertedNode){
    if(insertedNode == this->root){  //the case where the node is inserted as root
      insertedNode->setColor(black);
      //base case 1
    }
    else if(insertedNode->getParent()->getColor() == black){
      //do nothing, the tree remains valid after the insertion
      //base case 2
    }
    else if(insertedNode->getParent()->getColor() == red){
      //recursion case
      //if the parent is red, we have two cases:
      //first, the uncle is red 
      //second, the uncle is NULL or black
      if(insertedNode->getParent()->getKey()<insertedNode->getParent()->getParent()->getKey()){
        //in this case, the uncle is the right child of the grandparent

        if(insertedNode->getParent()->getParent()->getRight() == NULL){ //uncle does not exist
          if(insertedNode->getKey() < insertedNode->getParent()->getKey()){
            rotationcase1(insertedNode);
          }
          else if(insertedNode->getKey() > insertedNode->getParent()->getKey()){
            rotationcase2(insertedNode);
          }
        }

         else if(insertedNode->getParent()->getParent()->getRight()->getColor() == red){
          recolor(insertedNode);
          makeValid(insertedNode->getParent()->getParent());
        }
        else{
          if(insertedNode->getKey() < insertedNode->getParent()->getKey()){
            rotationcase1(insertedNode); //inserted node is the left child of parent, parent is the left child of grandparent
          }
          else if(insertedNode->getKey() > insertedNode->getParent()->getKey()){
            rotationcase2(insertedNode); //inserted node is the right child of parent, parent is the left child of grandparent
          }
        }
      }
      else if(insertedNode->getParent()->getKey() > insertedNode->getParent()->getParent()->getKey()){
        // in this case, the uncle is the left child of the grandparent
        if(insertedNode->getParent()->getParent()->getLeft() == NULL){ //uncle does not exist
          if(insertedNode->getKey() < insertedNode->getParent()->getKey()){
            rotationcase4(insertedNode);
          }
          else if(insertedNode->getKey() > insertedNode->getParent()->getKey()){
            rotationcase3(insertedNode);
          }
        }
        else if(insertedNode->getParent()->getParent()->getLeft()->getColor() == red){  //uncle is red
          recolor(insertedNode);
          makeValid(insertedNode->getParent()->getParent());
        }
        else{     //uncle is black
          if(insertedNode->getKey() < insertedNode->getParent()->getKey()){
            rotationcase4(insertedNode); //inserted node is the left child of parent, parent is the right child of grandparent
          }
          else if(insertedNode->getKey() > insertedNode->getParent()->getKey()){
            rotationcase3(insertedNode); //inserted node is the right child of parent, parent is the right child of grandparent
          }
        }
      }
    }
  }

  void recolor(RedBlackNode<KeyType, ValueType>* & insertedNode){
    insertedNode->getParent()->getParent()->setColor(red);
    insertedNode->getParent()->getParent()->getLeft()->setColor(black);
    insertedNode->getParent()->getParent()->getRight()->setColor(black);
  }

  void rotationcase1(RedBlackNode<KeyType, ValueType>* insertedNode){ //inserted node is the left child of parent, parent is the left child of grandparent
    RedBlackNode<KeyType, ValueType>*  old_grandparent = insertedNode->getParent()->getParent();
    RedBlackNode<KeyType, ValueType>*  aborted_right_child = insertedNode->getParent()->getRight();
    if(this->root == old_grandparent){
      //if grandparent is root node
      this->root = insertedNode->getParent();
      this->root->setParent(NULL);
      insertedNode->getParent()->setRight(old_grandparent);
      old_grandparent->setParent(insertedNode->getParent());
      old_grandparent->setLeft(aborted_right_child);
      if(aborted_right_child!=NULL){
        aborted_right_child->setParent(old_grandparent);
      }
    }
    else if(insertedNode->getParent()->getParent()->getKey() < insertedNode->getParent()->getParent()->getParent()->getKey()){
      //if grandparent is the left child of grandparent's parent
      old_grandparent->getParent()->setLeft(insertedNode->getParent());    
      insertedNode->getParent()->setRight(old_grandparent);
      insertedNode->getParent()->setParent(old_grandparent->getParent());
      old_grandparent->setParent(insertedNode->getParent());
      old_grandparent->setLeft(aborted_right_child);
      if(aborted_right_child!=NULL){
        aborted_right_child->setParent(old_grandparent);
      }
    }
    else{
      //if grandparent is the right child of grandparent's parent
      old_grandparent->getParent()->setRight(insertedNode->getParent());
      insertedNode->getParent()->setRight(old_grandparent);
      insertedNode->getParent()->setParent(old_grandparent->getParent());
      old_grandparent->setParent(insertedNode->getParent());
      old_grandparent->setLeft(aborted_right_child);
      if(aborted_right_child!=NULL){
        aborted_right_child->setParent(old_grandparent);
      }
    }
    //then set the new grandparent's color to black, old grandparent's color to red
    old_grandparent->setColor(red);
    insertedNode->getParent()->setColor(black);
  }
  void rotationcase2(RedBlackNode<KeyType, ValueType>*  insertedNode){//inserted node is the right child of parent, parent is the left child of grandparent
    //do a single rotation
    RedBlackNode<KeyType, ValueType>*  old_parent = insertedNode->getParent();
    insertedNode->setParent(old_parent->getParent());
    insertedNode->getParent()->setLeft(insertedNode);
    old_parent->setParent(insertedNode);
    old_parent->setRight(insertedNode->getLeft());
    if(insertedNode->getLeft()!=NULL){
      insertedNode->getLeft()->setParent(old_parent);
    }
    insertedNode->setLeft(old_parent);
    //now the situation becomes case1, so we perform a case1 rotation
    rotationcase1(insertedNode->getLeft());
  }
  void rotationcase3(RedBlackNode<KeyType, ValueType>* insertedNode){//inserted node is the right child of parent, parent is the right child of grandparent
     RedBlackNode<KeyType, ValueType>*  old_grandparent = insertedNode->getParent()->getParent();
     RedBlackNode<KeyType, ValueType>*  aborted_left_child = insertedNode->getParent()->getLeft();
     if(this->root == old_grandparent){
       //if grandparent is root node
       this->root = insertedNode->getParent();
       this->root->setParent(NULL);
       insertedNode->getParent()->setLeft(old_grandparent);
       old_grandparent->setParent(insertedNode->getParent());
       old_grandparent->setRight(aborted_left_child);
       if(aborted_left_child!=NULL){
        aborted_left_child->setParent(old_grandparent);
       }
     }
     else if(old_grandparent->getKey() < old_grandparent->getParent()->getKey()){
      //if grandparent is the left child of grandparent's parent
      old_grandparent->getParent()->setLeft(insertedNode->getParent());
      insertedNode->getParent()->setLeft(old_grandparent);
      insertedNode->getParent()->setParent(old_grandparent->getParent());
      old_grandparent->setParent(insertedNode->getParent());
      old_grandparent->setRight(aborted_left_child);
      if(aborted_left_child!=NULL){
        aborted_left_child->setParent(old_grandparent);
      }
     }
     else{
      //if grandparent is the right child of grandparent's parent
      old_grandparent->getParent()->setRight(insertedNode->getParent());
      insertedNode->getParent()->setLeft(old_grandparent);
      insertedNode->getParent()->setParent(old_grandparent->getParent());
      old_grandparent->setParent(insertedNode->getParent());
      old_grandparent->setRight(aborted_left_child);
      if(aborted_left_child!=NULL){
        aborted_left_child->setParent(old_grandparent);
      }
     }
     //then set the new grandparent's color to black, old grandparent's color to red
     old_grandparent->setColor(red);
     insertedNode->getParent()->setColor(black);
  }

  void rotationcase4(RedBlackNode<KeyType, ValueType>* insertedNode){//inserted node is the left child of parent, parent is the right child of grandparent
     //do a single rotation
    RedBlackNode<KeyType, ValueType>*  old_parent = insertedNode->getParent();
    insertedNode->setParent(old_parent->getParent());
    insertedNode->getParent()->setRight(insertedNode);
    old_parent->setParent(insertedNode);
    old_parent->setLeft(insertedNode->getRight());
    if(insertedNode->getRight()!=NULL){
      insertedNode->getRight()->setParent(old_parent);
    }
    insertedNode->setRight(old_parent);
    //now the situation becomes case3, so we perform a case3 rotation
    rotationcase3(insertedNode->getRight());
  }
};

#endif
