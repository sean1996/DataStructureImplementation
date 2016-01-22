#include "hashtable.h"
#include <stdexcept>
using namespace std;



//hash function
const long long HashTable::hash_function(const string s) const{
    string temp = s;
	long long r = 0;
	int exponent = 0;
    char current;
	while(temp.size() > 0){
        current = temp[temp.size()-1];
        // if((!(current>='A' && current<='Z')) && (!(current>='a' && current<='z'))){
        //     throw invalid_argument("the word should not contain any non-word character");
        // }
        if(current >= 'A' && current <= 'Z'){
            current = current - ('A' - 'a');
        }
        int digitValue = (int)current;
        digitValue = digitValue-97; //convert it to 0-26
		r += digitValue*pow(29, exponent);
		exponent += 1;
		temp = temp.substr(0, temp.size()-1);
	}
	return r;
}
//resize function
void HashTable::resize(){
    list<pair<string, int> >** new_data;
	int NumOfIndex_old = NumOfIndex;
	NumOfIndex = NumOfIndex*2 + 1;
	new_data = new list<pair<string, int> >*[NumOfIndex];
	for(int i=0; i< NumOfIndex; i++){
		new_data[i] = new list<pair<string, int> >;
	}
	//rehash all the elements from the old data list into the new one
	for(int i=0; i< NumOfIndex_old; i++){
		for(list<pair<string, int> >::iterator it = data[i]->begin();
    	it != data[i]->end();
    	it++){
			long long index = hash_function(it->first);
   			index = index % NumOfIndex;
   			//since the old array is garantee to be free with dupilicated keys,
   			//we don't have to check for duplicates key here
   			new_data[index]->push_back(*it);
    	}
	}
	//deallocate the old array and assign the data pointer to the new array
	for(int i=0; i< NumOfIndex_old; i++){
		delete data[i];
	}
	delete[] data;
	data = new_data;
}

/* Constructor that builds a hash table with 31 indices. */
HashTable::HashTable (){
	NumOfElement = 0;
	NumOfIndex = 31;
	data = new list<pair<string, int> >*[NumOfIndex];

	for(int i=0; i<NumOfIndex; i++){
		data[i] = new list<pair<string, int> >;
	}

}	

//destructor
HashTable::~HashTable (){
	for (int i=0; i<NumOfIndex; i++){
		delete data[i];
	}
	delete[] data;
}

void HashTable::add (std::pair<std::string, int> new_item){
 /* adds new_item to the hashtable.  
    Conflicts should be resolved via chaining.
    You may use the STL list to accomplish this. 
    Throws an exception if the key is already 
    in the hashtable.*/
    long long hashIndex = hash_function(new_item.first);
    long long tempIndex = hashIndex;    //this is a temp copy of hashIndex in case the NumOfIndex is resized
    hashIndex = hashIndex % NumOfIndex; 
    //search if the elemet already exists
    for(list<pair<string, int> >::iterator it = data[hashIndex]->begin();
    	it != data[hashIndex]->end();
    	it++){
    	if(it->first == new_item.first){
    		throw invalid_argument("existing key");
    	}
    }
	//check if the hashtable is at its maximum load capacity
	if(((double)(NumOfElement+1))/NumOfIndex > 1){
		resize();
		hashIndex = tempIndex % NumOfIndex;
	}
	data[hashIndex]->push_back(new_item);
    NumOfElement+=1;
 }


const int& HashTable::find(std::string key) const{
 /* returns the value with the associated key. 
    Throws an exception if the key is not in the
    hashtable. */
    long long hashIndex = hash_function(key);
    hashIndex = hashIndex % NumOfIndex;
    ///search if the key exists in the list
    for(list<pair<string, int> >::iterator it = data[hashIndex]->begin();
    	it != data[hashIndex]->end();
    	it++){
    	if(it->first == key){
    		return it->second;
    	}
    }
    //the key does not exist in the array
    throw invalid_argument("key does not exist");
}

void HashTable::remove (std::string key){
 /* removes the element with the associated key. 
    Throws an exception if the key is not in the
    hashtable. */
    long long hashIndex = hash_function(key);
    hashIndex = hashIndex % NumOfIndex;
    ///search if the key exists in the list
    for(list<pair<string, int> >::iterator it = data[hashIndex]->begin();
    	it != data[hashIndex]->end();
    	it++){
    	if(it->first == key){
    		data[hashIndex]->erase(it);
    		return;
    	}
    }
    //the key does not exist in the array
    throw invalid_argument("key does not exist");
}

void HashTable::update (std::pair<std::string, int> new_value){
 /* finds the item with the associated key, 
    and updates its value accordingly. 
    Throws an exception if the key is not in the
    hashtable. */
    long long hashIndex = hash_function(new_value.first);
    hashIndex = hashIndex % NumOfIndex;
    ///search if the key exists in the list
    for(list<pair<string, int> >::iterator it = data[hashIndex]->begin();
    	it != data[hashIndex]->end();
    	it++){
    	if(it->first == new_value.first){
    		it->second = new_value.second;
    		return;
    	}
    }
    //the key does not exist in the array
    throw invalid_argument("key does not exist");
 }