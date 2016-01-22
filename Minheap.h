#ifndef MINHEAP_H
#define MINHEAP_H

#include "hashtable.h"
#include <vector>
#include <string>

using namespace std;

class MinHeap {
     public:
       MinHeap (int d);
       /* Constructor that builds a d-ary Min Heap
          This should work for any d >= 2,
          but doesn't have to do anything for smaller d.*/

       ~MinHeap ();

       void add (string item, int priority);
         /* adds the item to the heap, with the given priority. */

       const string & peek () const;
         /* returns the element with smallest priority.  If
            multiple items have the smallest priority, it returns
            the string which comes  alphabetically.
            Throws an exception if the heap is empty. */

       void remove ();
         /* removes the element with smallest priority.  If
            multiple items have the smallest priority, it removes
            the string which comes first alphabetically.
            Throws an exception if the heap is empty. */

       void update (string item, int priority);
         /* updates the priority for the specified element.
            You may want this function to do nothing if the new 
            priority is higher than the old one.
            Throws an exception if the item is not in the heap. */  

       bool isEmpty ();
         /* returns true iff there are no elements on the heap. */

   private:
      // whatever you need to naturally store things.
      // You may also add helper functions here.
       int NumOfChild;
       vector<pair<string, int> > data;
       HashTable indexMap;

      //heloper function that compares the priority of two strings, if two strings have the same priority, 
      //return if the first string is larger alphabetically 
      bool doPumpUp(int first, int second);

      //helper function that pump the inserted element to the postion it is supposed to belong to.
      void pump_up(int position);

      //helper function that push the inserted element down to the postion it is supposed to belong to.
      void push_down(int position);

  };


#endif