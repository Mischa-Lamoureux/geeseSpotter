//The functions I need to create
#include "geesespotter_lib.h"

//To run program:
//                g++ -o main geesespotter.cpp geesespotter_lib.cpp -std=c++11

//Allocate a char array with xdim * ydim elements and initialize each element to zero
char * createBoard(std::size_t xdim, std::size_t ydim) { //EARLY SUBMISSION

    std::size_t size_of_array{xdim * ydim};

    //Allocate a new arrat of size xdim * ydim
    char *new_array{new char [size_of_array]{}};

    //Set each entry in the array to 0
    for(std::size_t i{0}; i < size_of_array; ++i) {
        new_array[i] = 0;
    }

    //Return new_array to get stored in *board in main
    return new_array;
}

void computeNeighbors(char * board, std::size_t xdim, std::size_t ydim) {

    std::size_t index{0};
    std::size_t max_index{xdim*ydim};
    //SHOULD PROBABLY CHANGE THIS TO STD::SIZE_T!!!
    int check_field{0};
    int neighbor_counter{0};

    //Cycle through each entry of the array and check all 8 spots around each field
    for(int y{0}; y < ydim; ++y) {
        for(int x{0}; x < xdim; ++x) {

            //If accessed index is != 9, continue
            if((board[index] & valueMask()) != 9) {
                for(int j{(y - 1)}; j <= (y + 1); ++j) {
                    for(int i{(x - 1)}; i <= (x + 1); ++i) {
                        check_field = ((j*xdim) + i);
                        //If the check_field is not outside of the array, continue
                        if( !( (i < 0) || (i >= xdim) || (j < 0) || (j >= ydim) || ((i == x) && (j == y) ) ) ) {
                            //If the value of check_field is = 9, increment the neighbor counter
                            if((board[check_field] & valueMask()) == 9) {
                                ++neighbor_counter;
                            }
                        } 
                    }
                }
            //Changes the last 4 bits to the appropriate number of neighbors
            board[index] = (board[index] | neighbor_counter);
            //Reset counter
            neighbor_counter = 0;
            }
            ++index;
        }
    }
}

//Hide all the field values
void hideBoard(char * board, std::size_t xdim, std::size_t ydim) { //EARLY SUBMISSION

    std::size_t size_of_array{xdim * ydim};

    //Sets each entry in the array to "hidden" (xx1xxxxx)
    for(std::size_t i{0}; i < size_of_array; ++i) {
        board[i] = board[i] | hiddenBit();
    }
}

//Deallocate the given board
void cleanBoard(char * board) { //EARLY SUBMISSION
    
    delete[] board;
    board = nullptr;
}

//Print the content of the board.
//Note that the only time a field with a goose is displayed is at the end of the game (either lost or won)
void printBoard(char * board, std::size_t xdim, std::size_t ydim) { //EARLY SUBMISSION

    std::size_t index{0};

    for(int j{0}; j < ydim; ++j) {
        for(int i{0}; i < xdim; ++i) {
            //If entry has hiddenBit = 1
            if(board[index] & markedBit()) {
                std::cout << "M";
            }
            //If entry has markedBit = 1
            else if(board[index] & hiddenBit()) {
                std::cout << "*";
            }
            //If entry has neither hiddenBit nor markedBit
            else {
                std::cout << (int)board[index];
            }
            ++index;
        }
        std::cout << std::endl;
    }
}

int reveal(char * board, std::size_t xdim, std::size_t ydim, std::size_t xloc, std::size_t yloc) {

    //Calculates the index based on the y-location, x-location, and x-input
    std::size_t index{(yloc*xdim) + xloc};
    //Casts yloc and xloc to integers that can be used in the following for loops
    int y{(int)yloc};
    int x{(int)xloc};
    std::size_t manipulate_field{0};

    //Check if field is already marked
    if(board[index] & markedBit()) {
        return 1;
    }
    //Check if field is already revealed
    else if( !(board[index] & hiddenBit()) ) {
        return 2;
    }
    //Check if field contains a goose
    else if((board[index] & valueMask()) == 9) {
        board[index] = (board[index] ^ hiddenBit());
        return 9;
    } 
    //Check if field contains a 0 value
    else if( !(board[index] & valueMask()) ) {
        for(int j{y - 1}; j <= (y + 1); ++j) {
            for(int i{x - 1}; i <= (x + 1); ++i) {
                manipulate_field = ((j*xdim) + i);
                //If the manipulate_field is not outside of the array, continue
                if( !( (i < 0) || (i >= xdim) || (j < 0) || (j >= ydim) ) ) {
                    //Check if manipulatd_field is hidden
                    if (board[manipulate_field] & hiddenBit()) {
                        board[manipulate_field] = (board[manipulate_field] ^ hiddenBit());
                    }
                }
            }
        }
        return 0;
    }
    //Activates if the field is not marked, not revealed, and has a value other than 0 or 9
    else {
        board[index] = (board[index] ^ hiddenBit());
        return 0;
    }
}

//Mark/unmark the given field. Return 2 if the field is already revealed. Return 0 otherwise.
int mark(char * board, std::size_t xdim, std::size_t ydim, std::size_t xloc, std::size_t yloc) { //EARLY SUBMISSION
    
    //Calculates the index based on the y-location, x-location, and x-input
    std::size_t index{(yloc*xdim) + xloc};

    //If board[index] does not have hiddenBit = 1
    if(!(board[index] & hiddenBit())) {
        return 2;
    } else {
        //If board[index] is already marked
        if(board[index] & markedBit()) {
            //Unmark board[index]
            board[index] = (board[index] ^ markedBit());
        } else {
            //Mark board[index]
            board[index] = (board[index] | markedBit());
        }
        return 0;
    }
}

bool isGameWon(char * board, std::size_t xdim, std::size_t ydim) {

    std::size_t size_of_array{xdim*ydim};

    for(std::size_t i{0}; i < size_of_array; ++i) {
        //If board does not equal 9 and board is unhidden, or the board equals 9, continue
        if(((board[i] & valueMask()) != 9) && !(board[i] & hiddenBit()) || ((board[i] & valueMask()) == 9)) {
        } else {
            return false;
        }
    }
    return true;
}