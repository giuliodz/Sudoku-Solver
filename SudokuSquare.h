#ifndef SUDOKUSQUARE_H
#define SUDOKUSQUARE_H

// Do not add any #include statements to this file

class SudokuSquareSet {

    // TODO: write your code here
protected:
	unsigned int set;
	int possibleNumbers;

public:
	SudokuSquareSet():
		set(0), possibleNumbers(0){}

	/*
		@return the set of this ssquare.
	*/
	const unsigned int getSet() const{
		return set;
	}


	/*
		@return how many values this set is holding.
	*/
	int size() const{
		return possibleNumbers;
	}


	/*
		@return true if the square has no values set.
	*/
	bool empty(){
		return (possibleNumbers == 0);
	}


	/*
		Method to empty all the values from the square.
	*/
	void clear(){
		set = 0;
		possibleNumbers =0;
	}


	/*
		@return true if two sets are holding the same values.
	*/
	bool operator==(SudokuSquareSet & other){
		return (other.getSet() == set);
	}


	/*
		@return true if two sets are not holding the same values.
	*/
	bool operator!=(SudokuSquareSet & other){
		return (other.getSet() != set);
	}

	
	class Iterator{
		protected:
			int position;//index of the value which is current visited.
			int value;// 1 if there is a value at that position. 0 otherwise.
			SudokuSquareSet * square;// the visited SudokuSquareSet
		public:
			/*
				@param the SudokuSwuareSet to visit.
				@param the index of the index of the set that we want to look at.
			*/
			Iterator(SudokuSquareSet & x, const int & index){
				square = &x;
				position = index;
				value = (square->set >> index) & 1UL; 
			}


			/*
				Method that allows to go to ne next value present in the SudokuSquareSet.
			*/
			void operator++(){
				position++;
				value = (square->set >> position) & 1UL; 
			}


			/*
				@return the value held at that position. if no value is held at this position, it recursively
				returns the value at the next position.
			*/
			int operator*(){
				if(value == 1){
					return position +1;
				}else{
					position++;
					value = (square->set >> position) & 1UL;
					return operator*();
				}
			}


			/*
				@param other is the iterator that we want to compare with.
				@return True if the two iterator are not visiting the element at the same index.
			*/
			bool operator!= (const Iterator & other){
				return (position != other.position);
			}

			/*
				@param other is the iterator that we want to compare with.
				@return True if the two iterator are visiting the element at the same index.
			*/
			bool operator== (const Iterator & other){
				return (position == other.position);
			}
	};

	
	/*
		@return an iterator visiting the first elemnt in the set.
	*/
	Iterator begin(){
		return Iterator(*this, 0);
	}


	/*	
		@return an iterator just off the last value present in the set.
	*/
	Iterator end(){
		int highest = findHighestValue();
		return Iterator(*this, highest);
	}


	/*
		@return The highest value held by the set.
	*/
	int findHighestValue(){
		int highest =0;
		for(int i =0; i<16; ++i){

			int value = (set >> i) & 1UL;
			if(value == 1){
			
				highest = (i+1);
			}
		}
		return highest;
	}



	/*
		Method that insert a given value in the set at the right index.
		@return an iterator visiting that newly inserted value.
	*/
	Iterator insert(const int & value){
			set |= 1UL << (value-1);
			possibleNumbers++; 
			return Iterator(*this, value-1);
	}


	/*
		@param value is the value to look for in the set.
		@return an iterator pointing to that value assuming that it is in the set. If that is not the case, it returns end().
	*/
	Iterator find(const int &  value){

		int bit = (set >> (value-1)) & 1UL;
		if (bit == 1){
			return Iterator(*this, value-1);
		}
		else{
     		return end();
		}	
	}

	/*
		@param value is the value to erase from the set.
		@returns 1 if the value was in the set. 0 otherwise.
	*/
	int erase(const int & value){
			int bit = (set >> (*this, value-1)) & 1U;
			if(bit ==1){
				set ^= 1UL << (*this, value-1);
				--possibleNumbers;
				return 1;
			}
			else return 0;

	}

	/*
		@param it is an iterator pointing to an element in the set that must be erased.
		@return 1 if the value was in the set. 0 otherwise.
	*/
	int erase(Iterator & it){
		int value = *it;
		return erase(value);

	}


};


// Do not write any code below this line
static_assert(sizeof(SudokuSquareSet) == sizeof(unsigned int) + sizeof(int), "The SudokuSquareSet class needs to have exactly two 'int' member variables, and no others");


#endif
