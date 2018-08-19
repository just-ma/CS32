#ifndef History_h
#define History_h

#include "globals.h"

class History
{
public:
    History(int nRows, int nCols); //standard public interface
    
    bool record(int r, int c);
    void display() const;
private:
    int m_nRows; //number of rows and columns are important
    int m_nCols;
    int displayGrid[MAXROWS][MAXCOLS]; //matrix of integers to record the number of turns a poisoned rat has been on a cell.
};

#endif /* History_h */
