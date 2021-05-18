#ifndef TOMBSTONES_H
#define TOMBSTONES_H
#include <iostream>
#include <stdlib.h>

template <class T>
class Tombstone
{
 public:
T* data;       				// pointer
int reference_count;			// keep count of objects 
bool isDead;				// for tombstones
Tombstone<T>(){ isDead = false; reference_count = 1; data = nullptr; } 
Tombstone<T>(T* pT){ isDead = false; reference_count = 1; data = pT;}
};


template <class T>
class Pointer {
public:
	Pointer<T>() {tombstone = new Tombstone<T>();}                       	  	 // default constructor
	
	// copy constructor
	Pointer<T>(const Pointer<T>& t){
	if(t.tombstone->isDead != true){
	tombstone = t.tombstone;
	++tombstone->reference_count;
	}else{
		tombstone = new Tombstone<T>();
		}
	}			 	 
	
	Pointer<T>(T* pT){tombstone = new Tombstone<T>(pT);}                 	  	// bootstrapping constructor
	 	//The parameterfor the bootstrapping constructor should always be
		// a call to operator new.
	
	~Pointer<T>(){--tombstone->reference_count;}                        	// destructor
	
	// dereferencing 
	T& operator*() const {
	if(tombstone->isDead){
		std::cout << "Error: dangling pointer!!" << std::endl;
		exit(1);
		}else if(tombstone->reference_count <= 0){
			std::cout << "Error: avoided memory leak!!" << std::endl;
			exit(1);
		}
		return *(tombstone->data);
	}		    		  
	
	// field dereferencing
	T* operator->() const{if(tombstone->isDead){
		std::cout << "Error: dangling pointer!!" << std:: endl;
		exit(1);
		}else if(tombstone->reference_count <= 0){
			std::cout << "Error:avoided memory leak!!" << std::endl;
				exit(1);}
		return this->tombstone->data;
		}                			
	
	// assignment
	Pointer<T>& operator=(const Pointer<T>& t){
	if(tombstone != t.tombstone){
		--tombstone->reference_count;
		tombstone = t.tombstone;
		++tombstone->reference_count;
		}
		return *this;
	}   		 	


	friend void free(Pointer<T>& t){
	if(t.tombstone->data != nullptr){
		delete t.tombstone->data;
		t.tombstone->data = nullptr;
		t.tombstone->isDead = true;
		}
		--t.tombstone->reference_count;
		t.tombstone = nullptr;
	}           			 	// delete pointed-at object
		// This is essentially the inverse of the new inside the call to
		// the bootstrapping constructor.It should delete the pointed-to
		// object (which should in turn call its destructor).

		// equality comparisons:

	bool operator==(const Pointer<T>& t) const{
		return t.tombstone->data == tombstone->data;}

	bool operator!=(const Pointer<T>& t) const{
	return t.tombstone->data != tombstone->data;}

	bool operator==(const int n) const{
	return (tombstone->data == nullptr) && (n == 0);}
		// true if Pointer is null and int is zero

	bool operator!=(const int n) const{
	return (tombstone->data != nullptr) && (n != 0);}
		// false if Pointer is null and int is zero

Tombstone<T>* tombstone;	// Tombstone pointer
};

template <class T>
bool operator==(const int n, const Pointer<T>& t){ return t == n; }

template <class T>
bool operator!=(const int n, const Pointer<T>& t){ return t != n; }
#endif // TOMBSTONES_H