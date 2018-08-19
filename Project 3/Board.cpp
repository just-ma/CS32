#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>

#include <vector>

using namespace std;

class BoardImpl
{
  public:
    BoardImpl(const Game& g);
    void clear();
    void block();
    void unblock();
    bool placeShip(Point topOrLeft, int shipId, Direction dir);
    bool unplaceShip(Point topOrLeft, int shipId, Direction dir);
    void display(bool shotsOnly) const;
    bool attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId);
    bool allShipsDestroyed() const;

  private:
    struct Info
    {
        int ID, seg; //keeps track of ship information including point, direction
        Point p;
        Direction dir;
    };
    const Game& m_game;
    vector<Info> m_placed;
    char m_grid[MAXROWS][MAXCOLS];
};

BoardImpl::BoardImpl(const Game& g)
 : m_game(g)
{
    clear(); //clear grid (set all positions to '.')
}

void BoardImpl::clear()
{
    for (int r = 0; r < m_game.rows(); r++) //go through entire grid and set all values to '.'
    {
        for (int c = 0; c < m_game.cols(); c++)
        {
            m_grid[r][c] = '.';
        }
    }
}

void BoardImpl::block()
{
      // Block cells with 50% probability
    for (int r = 0; r < m_game.rows(); r++)
    {
        for (int c = 0; c < m_game.cols(); c++)
        {
            if (randInt(2) == 0)
            {
                m_grid[r][c] = 'X'; //turns blocks into 'X' which is a character ships can't use
            }
        }
    }
}

void BoardImpl::unblock()
{
    for (int r = 0; r < m_game.rows(); r++)
    {
        for (int c = 0; c < m_game.cols(); c++)
        {
            if (m_grid[r][c] == 'X') //replaces all blocks with water (".")
            {
                m_grid[r][c] = '.';
            }
        }
    }
}

bool BoardImpl::placeShip(Point topOrLeft, int shipId, Direction dir)
{
    if (shipId > m_game.nShips()) //checks if ID is valid (inside the range)
        return false;
    for (int n = 0; n < m_placed.size(); n++) //checks if the ship ID has already been placed
    {
        if (m_placed[n].ID == shipId)
            return false;
    }
    
    if (!m_game.isValid(topOrLeft)) //checks if topOrLeft is inside the grid
        return false;
    
    Info i;
    i.dir = dir;
    i.ID = shipId;
    i.seg = m_game.shipLength(shipId);
    i.p = topOrLeft;
    
    if (dir == HORIZONTAL) //for horizontal case...
    {
        if (topOrLeft.c + m_game.shipLength(shipId) > m_game.cols()) //checks if the length won't
            return false;                                            // make it go off the board
        for( int n = 0; n < m_game.shipLength(shipId); n++)
        {
            if (m_grid[topOrLeft.r][topOrLeft.c + n] != '.')  //checks if the path is free
                return false;
        }
        for( int n = 0; n < m_game.shipLength(shipId); n++)  //draws ship onto grid
        {
            m_grid[topOrLeft.r][topOrLeft.c + n] = m_game.shipSymbol(shipId);
        }
        m_placed.push_back(i);
        return true;
    }
    
    else //same as above, but for vertical case
    {
        if (topOrLeft.r + m_game.shipLength(shipId) > m_game.rows())
            return false;
        for( int n = 0; n < m_game.shipLength(shipId); n++)
        {
            if (m_grid[topOrLeft.r + n][topOrLeft.c] != '.')
                return false;
        }
        for( int n = 0; n < m_game.shipLength(shipId); n++)
        {
            m_grid[topOrLeft.r + n][topOrLeft.c] = m_game.shipSymbol(shipId);
        }
        m_placed.push_back(i);
        return true;
    }
}

bool BoardImpl::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{
    if (!m_game.isValid(topOrLeft)) //checks if the point is valid (within the grid)
        return false;
    
    bool check = false;
    int position = 0;
    for ( int n = 0; n < m_placed.size(); n++) //check if the shipID has been placed
    {
        if (m_placed[n].ID == shipId) // goes through vector of placed ships
        {
            check = true;
            position = n;
        }
    }
    if (!check)
        return false;
    
    if (dir == HORIZONTAL) //if the direction is horizontal...
    {
        if (topOrLeft.c + m_game.shipLength(shipId) > m_game.cols()) //checks if the length won't
            return false;                                             //go past the boundary
        for ( int n = 0; n < m_game.shipLength(shipId); n++)
        {
            //checks if every point from topOrLeft to the length of the ship contains the symbol
            if (m_grid[topOrLeft.r][topOrLeft.c + n] != m_game.shipSymbol(shipId))
                return false;
        }
        for ( int n = 0; n < m_game.shipLength(shipId); n++)
        {
            m_grid[topOrLeft.r][topOrLeft.c + n] = '.'; //actually unplaces the ship from the board
        }
        m_placed.erase(m_placed.begin() + position); //erase placed ship from vector
        return true;
    }
    else //does the same thing above but for Vertical condition
    {
        if (topOrLeft.r + m_game.shipLength(shipId) > m_game.rows())
            return false;
        for ( int n = 0; n < m_game.shipLength(shipId); n++)
        {
            if (m_grid[topOrLeft.r + n][topOrLeft.c] != m_game.shipSymbol(shipId))
                return false;
        }
        for ( int n = 0; n < m_game.shipLength(shipId); n++)
        {
            m_grid[topOrLeft.r + n][topOrLeft.c] = '.';
        }
        return true;
    }
}
    

void BoardImpl::display(bool shotsOnly) const
{
    cout<<"  "; //two spaces
    for (int n = 0; n < m_game.cols(); n++)
    {
        cout << n; //column numbering
    }
    cout<<endl;
    for (int r = 0; r < m_game.rows(); r++) //print out rows one at a time
    {
        cout<< r <<" ";                          //prints row number and a space
        for (int c = 0; c < m_game.cols(); c++)
        {
            if (!shotsOnly)
                cout<<m_grid[r][c]; //if shotsOnly is false, print exactly what the grid has
            else                    //if shotsOnly is true, only print hits and misses - no ships
            {
                if (m_grid[r][c] == 'o' || m_grid[r][c] == 'X')
                    cout<<m_grid[r][c];
                else
                    cout<<'.';
            }
        }
        cout<<endl;
    }
}

bool BoardImpl::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
    shotHit = false;
    shipDestroyed = false;
    if (!m_game.isValid(p)) //checks if the point is valid (within the grid)
        return false;
    else if (m_grid[p.r][p.c] == 'X' || m_grid[p.r][p.c] == 'o')
        return false; // checks if the position has been attacked yet
    
    else if (m_grid[p.r][p.c] != '.') //if the shot hit a ship (the grid position wasn't '.')
    {
        shotHit = true;
        int num;
        
        for (int n = 0; n < m_placed.size(); n++) //finds which ship was hit
        {
            if (m_game.shipSymbol(m_placed[n].ID) == m_grid[p.r][p.c])
            {
                num = n;
                break;
            }
        }
        m_grid[p.r][p.c] = 'X'; //set the point hit to 'X'
        m_placed[num].seg -= 1;
        
        if (m_placed[num].seg == 0)
        {
            shipDestroyed = true;
            shipId = m_placed[num].ID; //sets shipID to the ID of the ship hit
        }
        return true;
    }
    else //if the shot landed in water...
    {
        m_grid[p.r][p.c] = 'o';
        return true; //return true despite not hitting anything
    }
}

bool BoardImpl::allShipsDestroyed() const
{
    for (int n = 0; n < m_placed.size(); n++) //goes through all ships placed
    {
        if (m_placed[n].seg != 0) //if one of them is not sunken, return false
            return false;
    }
    return true; //else return true if all ships have sunk
}

//******************** Board functions ********************************

// These functions simply delegate to BoardImpl's functions.
// You probably don't want to change any of this code.

Board::Board(const Game& g)
{
    m_impl = new BoardImpl(g);
}

Board::~Board()
{
    delete m_impl;
}

void Board::clear()
{
    m_impl->clear();
}

void Board::block()
{
    return m_impl->block();
}

void Board::unblock()
{
    return m_impl->unblock();
}

bool Board::placeShip(Point topOrLeft, int shipId, Direction dir)
{
    return m_impl->placeShip(topOrLeft, shipId, dir);
}

bool Board::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{
    return m_impl->unplaceShip(topOrLeft, shipId, dir);
}

void Board::display(bool shotsOnly) const
{
    m_impl->display(shotsOnly);
}

bool Board::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
    return m_impl->attack(p, shotHit, shipDestroyed, shipId);
}

bool Board::allShipsDestroyed() const
{
    return m_impl->allShipsDestroyed();
}
