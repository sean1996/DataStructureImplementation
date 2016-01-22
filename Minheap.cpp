#include "Minheap.h"
#include <stdexcept>
using namespace std;

    MinHeap::MinHeap (int d){
       /* Constructor that builds a d-ary Min Heap
          This should work for any d >= 2,
          but doesn't have to do anything for smaller d.*/
        if(d>=2){
        	NumOfChild = d;
        }
    }

   MinHeap::~MinHeap (){

   }

   //heloper function that compares the priority of two strings, if two strings have the same priority, 
   //return if the first string is larger alphabetically 
   bool MinHeap::doPumpUp(int first, int second){
   	 if(data[first].second < data[second].second){
   	 	return true;
   	 }
   	 else if(data[first].second == data[second].second){
   	 	if(data[first].first.compare(data[second].first) < 0){
   	 		return true;
   	 	}
   	 	else {return false;}
   	 }
   	 else{
   	 	return false;
   	 }
   }

   //helper function that pump the inserted element to the postion it is supposed to belong to.
   void MinHeap::pump_up(int position){
   		int parent_position = (position-1)/NumOfChild;
   		if(position>0 && doPumpUp(position, parent_position)){
   			//swap the element in the data vector
   			pair<string, int> temp_parent = data[parent_position];
   			data[parent_position] = data[position];
   			data[position] = temp_parent;
   			//modify the index in the index map corredpondingly
   			indexMap.update(make_pair(data[parent_position].first, parent_position));
   			indexMap.update(make_pair(data[position].first, position));
   			//recursive on parents
   			pump_up(parent_position);
   		}

   }

   void MinHeap::add (string item, int priority){
     /* adds the item to the heap, with the given priority. */
   		pair<string, int> tobeinserted = make_pair(item, priority);
   		for(int i=0; i<data.size(); i++){
   			if(data[i].first == item){
   				throw invalid_argument("duplicated key");
   			}
   		}
   		data.push_back(tobeinserted);
   		indexMap.add(make_pair(item, (int)data.size()-1));
   		pump_up((int)data.size()-1);
   }

   const string & MinHeap::peek () const{
     /* returns the element with smallest priority.  If
        multiple items have the smallest priority, it returns
        the string which comes first alphabetically.
        Throws an exception if the heap is empty. */
        if(data.size() == 0){
        	throw invalid_argument("the heap is empty");
        }
        return data[0].first;
     }

     //helper function that push the inserted element down to the postion it is supposed to belong to.
    void MinHeap::push_down(int position){
    	if((int)data.size() > (position*NumOfChild + 1)){	//it is not a leaf node
    		int smallest_child_index = position*NumOfChild + 1;
    		//find the child with smallest priority and alphabetically first
    		for(int i=2; i <= NumOfChild; i++){
    			if((int)data.size() > position*NumOfChild+i){ // if this child exists
    				if(doPumpUp(position*NumOfChild+i, smallest_child_index)){
    					smallest_child_index = position*NumOfChild+i;
    				}
    			}
    		}
    		//if the smallest child is smaller than the parent
    		if(doPumpUp(smallest_child_index, position)){
    			pair<string, int> temp_parent = data[position];
    			data[position] = data[smallest_child_index];
    			data[smallest_child_index] = temp_parent;
    			indexMap.update(make_pair(data[position].first, position));
    			indexMap.update(make_pair(data[smallest_child_index].first, smallest_child_index));
    			push_down(smallest_child_index);
    		}

    	}

    }

   void MinHeap::remove (){
     /* removes the element with smallest priority.  If
        multiple items have the smallest priority, it removes
        the string which comes first alphabetically.
        Throws an exception if the heap is empty. */
        if(isEmpty()){
        	throw invalid_argument("the heap is empty");
        }
        indexMap.remove(data[0].first);
        data[0] = data[data.size()-1];
        data.pop_back();
        push_down(0);
     }

   void MinHeap::update (string item, int priority){
     /* updates the priority for the specified element.
        You may want this function to do nothing if the new 
        priority is higher than the old one.
        Throws an exception if the item is not in the heap. */ 
        int indexOfItem = indexMap.find(item);
        if(priority < data[indexOfItem].second){
        	data[indexOfItem].second = priority;
        	pump_up(indexOfItem);
        }
    } 

   bool MinHeap::isEmpty (){
     /* returns true iff there are no elements on the heap. */   
   	if(data.size() == 0){
   		return true;
   	}
   	else{
   		return false;
   	}
   }
