#ifndef SUDOKU_H
#define SUDOKU_H

#include "SudokuSquare.h"
#include "Searchable.h"
#include <vector>
#include <set>
#include <math.h>
#include <algorithm>
using namespace std;

// TODO: Your Sudoku class goes here:
class Sudoku : public Searchable{
private:
	vector<vector<SudokuSquareSet>> board;
	int SQBox;

	
public:
	/*
		Constructor that initialize the board to store a vetor of vectors of sets of given length, where each set is initialized to have all the possible 
		values from 1 to @param size.
	*/
	Sudoku(const int & size){
		SQBox = sqrt(size);

		for(int i = 0 ; i<size; ++i){
			vector<SudokuSquareSet> row;
			for(int z =0; z<size; ++z){

				SudokuSquareSet square;
				for(int j = 1; j<=size; ++j){
					square.insert(j);//initializing each set to have the values in setElements
				}
				row.push_back(square);
			}
			board.push_back(row);
		}
	}

	/*
		Set the board to be identical to another Sudoku's board.
		@param boardToCopy is the other Sudoku's board to be copied.
	*/
	void setBoard (const vector<vector<SudokuSquareSet>> & boardToCopy){
		board.clear();
		board = boardToCopy;
	}

	/*
		Checks if at a given square in board there is a set with only one element.
		If that's the case, it returns that element.
		otherwise it returns -1.
	*/
	int getSquare(const int & row, const int & column){
		SudokuSquareSet set = board[row][column];
		if (set.size() == 1){
			return *set.begin();
		}
		else{
			return -1;
		}
	}

	/*
		set a specific cell (set) to have only one @param value.
		Afterwards it rearranges the board so that when we have a cell with only one possible
		value, it erases that value from every cell in that @param row, @param column, and subox.
		If everything goes smooth it @return true.
		If during the rearrangement of the cells it incurs a cell with 0 possible values in it, it @return false.
	*/
	bool setSquare(const int & row, const int & column, const int & value){
		bool toReturn = true;
		if(board[row][column].find(value) != board[row][column].end()){
			board[row][column].clear();
			board[row][column].insert(value);
			
		}
		else{
			toReturn = false;
			return toReturn;
		}

		for (int i = 0; i<board.size(); ++i){
			
			for(int j = 0; j<board.size(); ++j){
				toReturn = checkValidity(i,j);
				if(!toReturn) return toReturn;
			}
		}

		return toReturn;

	}

	/*
		Given a certain square at @param row, and @param col, this method checks if it s valid. Being valid means that that cell is not empty, and
		if it has only one value no other cell in its row, column, or sub box has that value. if some has, removing it doesn't have to cause problems.
		It s a recursive method. It s basecases are : either that cell has still more than one value when checked (@return true), or
		that cell is empty when checked (@return false), or it has done all the needed checkings without any problem (@return true).   
	*/
	bool checkValidity(const int & row, const int & col){
		bool toReturn = true;

		if(board[row][col].size() > 1) return true;
		
		else if(board[row][col].empty())return false;
        else{
		//by now we know this cell has only one value;
		    
    		int value = *board[row][col].begin();
    		//doing the same check for each element on this row;
    		for(int i = 0; i<board.size(); ++i){
    		
    			if(i != col){
    				
    				if(board[row][i].size() >= 1){
                        
                        int e = board[row][i].erase(value);
    				    if(e > 0){
    				        
        				toReturn = checkValidity(row, i);
        				if (!toReturn) return toReturn;
    				    }
    				}
    			}
    		}
    		//doing the same check for each element on the same column.
    		for(int i = 0; i<board.size(); ++i){
    
    			if(i != row){
    				
    				if(board[i][col].size() >= 1){
    				
    				    int e = board[i][col].erase(value);
    				    if(e > 0){
    				        
            				toReturn = checkValidity(i, col);
            				if (!toReturn) return toReturn;
    				    }
    				}
    			}
    		}
    		//doing the same check for each cell in this cell's sub box.
    		int startCol = (floor(col/SQBox)) * SQBox;
    		int endCol = startCol + SQBox;
    		int startRow = (floor(row/SQBox)) * SQBox;
    		int endRow = startRow + SQBox;
    		for(int i = startRow; i < endRow; ++i){
    			
    			for(int j = startCol; j < endCol; ++j){
    	
    			    if(i != row || j != col){
    				
    				    int e = board[i][j].erase(value);
    				    
    				    if(e > 0){
    				    
    					    toReturn = checkValidity(i, j);
    					    if (!toReturn) return toReturn;
    				    }
    				}
    			}
    		}
	    }

		return toReturn;
	}
	

	/*
		@return true if the puzzle has been solved. This means that all the squares contain just one value.
	*/
	virtual bool isSolution() const{		
		for(int i = 0; i < board.size(); ++i){
    			
    		for(int j = 0; j < board.size(); ++j){

    			if(board[i][j].size() != 1){
    				return false;
    			}
    		}
    	}

		return true;
	}

	/*
		Debugging function to print the board to screen.
	*/
	virtual void write(ostream & o) const{
		string standardDivisor = " ";
		
		for(int i =0; i<(board.size()+1); ++i){
			standardDivisor += " ";
		}

		for(int i =0; i<board.size(); ++i){
			for(int j = 0; j<board.size(); ++j){

				int difference = board.size() - board[i][j].size();
				string divisor = standardDivisor.substr(0, difference);
				
				o<<divisor<<"{";
				const unsigned int square = board[i][j].getSet();
				for(int i =0; i<board.size(); ++i){

					int value = (square >> i) & 1UL;
					o<<value;
	            }
				
				o<<"}";
			}
			o<<endl;
		}
	}

	virtual vector<unique_ptr<Searchable>> successors() const{
		
		vector<unique_ptr<Searchable> > successors;
		pair<int,int> coordinate (-1,-1);
		
		for(int i = 0; i<board.size(); ++i){

			for(int j = 0; j<board.size(); ++j){
				
				if(board[i][j].size() > 1){

					coordinate.first = i;
					coordinate.second = j;
					break; 
				}

			}
			if(coordinate.first != -1)
					break;
		}

		auto square = board[coordinate.first][coordinate.second].getSet();
		for(int i =0; i<board.size(); ++i){

			int bit = (square >> i) & 1UL;
			if(bit == 1){

				int value = i+1;
				auto copy = new Sudoku(board.size());
				copy->setBoard(board);
				if(copy->setSquare(coordinate.first, coordinate.second, value)){

					successors.emplace_back(copy);
				}
				else{
					delete copy;
				}
			}

	    }

		return successors;
	}

	virtual int heuristicValue() const{
		return 1;
	}


};



#endif