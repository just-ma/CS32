#include "History.h"

History::History(int nRows, int nCols): m_nRows(nRows), m_nCols(nCols)
{
    for (int n = nRows; n>0; n--) //initializes all integers in the matrix to 0
    {
        for (int m = nCols; m>0; m--)
        {
            displayGrid[n][m] = 0;
        }
    }
};

bool History::record(int r, int c)
{
    if (r > m_nRows || r > m_nCols) //if the position is outside the matrix, return false
    {
        return false;
    }
    else
    {
        displayGrid[r][c]++; //else, add one to that specific position in the matrix
        return true;
    }
}

void History::display() const
{
    clearScreen();
    int r, c;
    
    // Fill displayGrid with dots (empty) and letters (number of times a poisoned rat has been there)
    for (r = 1; r <= m_nRows; r++)
    {
        for (c = 1; c <= m_nCols; c++)
        {
            if (displayGrid[r][c] == 0) //if the integer of the cell is 0, cout '.'
            {
                cout<<'.';
            }
            else if(displayGrid[r][c] > 0)
            {
                if (displayGrid[r][c] >25) //if the cell integer is greater than 25(Y), cout 'Z'
                {
                    cout<<'Z';
                }
                else
                    cout<<char('A'+displayGrid[r][c]-1); //cout characters 'A' to 'Y' depending on integer in cell
            }
            
        }
        cout<<endl;
    }
    cout<<endl;
}

