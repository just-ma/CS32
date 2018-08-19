#include "Game.h"
#include "Board.h"
#include "Player.h"
#include "globals.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <cctype>

#include <vector>

using namespace std;

class GameImpl
{
 public:
    GameImpl(int nRows, int nCols);
    int rows() const;
    int cols() const;
    bool isValid(Point p) const;
    Point randomPoint() const;
    bool addShip(int length, char symbol, string name);
    int nShips() const;
    int shipLength(int shipId) const;
    char shipSymbol(int shipId) const;
    string shipName(int shipId) const;
    Player* play(Player* p1, Player* p2, Board& b1, Board& b2, bool shouldPause);
private:
    int m_rows, m_cols;
    struct Ship //structure that keeps information of all ship including length, name, and symbol
    {
        int length;
        string name;
        char symbol;
    };
    vector<Ship> m_ships; //vector of Ship keeps track of all ships placed onto board
};

GameImpl::GameImpl(int nRows, int nCols)
{
    m_rows = nRows; //initialize rows and columns
    m_cols = nCols;
}

int GameImpl::rows() const
{
    return m_rows; //return rows
}

int GameImpl::cols() const
{
    return m_cols; //return cols
}

bool GameImpl::isValid(Point p) const
{
    return p.r >= 0  &&  p.r < rows()  &&  p.c >= 0  &&  p.c < cols();
    //checks if the point is within bounds of the board
}

Point GameImpl::randomPoint() const
{
    return Point(randInt(rows()), randInt(cols()));
    //generates random point
}

bool GameImpl::addShip(int length, char symbol, string name)
{
    Ship s;             //new ships with input length, symbol and name
    s.length = length;
    s.symbol = symbol;
    s.name = name;
    
    m_ships.push_back(s); //push ship back into vector
    return true;
}

int GameImpl::nShips() const
{
    return m_ships.size(); //return size of ship vector
}

int GameImpl::shipLength(int shipId) const
{
    return m_ships[shipId].length; //return length of ship with shipID
}

char GameImpl::shipSymbol(int shipId) const
{
    return m_ships[shipId].symbol;; //return symbol of ship with shipID
}

string GameImpl::shipName(int shipId) const
{
    return m_ships[shipId].name;;  //return name of ship with shipID
}

Player* GameImpl::play(Player* p1, Player* p2, Board& b1, Board& b2, bool shouldPause)
{
    if(p1->isHuman()) //if human, output statements to ask to place their ships
    {
        cout<<p1->name()<<" must place "<<nShips()<<" ships."<<endl;
        p1->placeShips(b1);
    }
    else
    {
        if(!(p1->placeShips(b1))) //initialize the boards
            return nullptr; //if initialization failed, return nullptr (unable to place ships)
    }
    if(p2->isHuman()) //if human, output statements to ask to place their ships
    {
        cout<<p2->name()<<" must place "<<nShips()<<" ships."<<endl;
        p2->placeShips(b2);
    }
    else
    {
        if(!(p2->placeShips(b2)))  //do the same for the other board
            return nullptr;
    }
    bool shotHit, shipDestroyed;
    int shipID;
    
    while (true) //repeatedly...
    {
        cout << p1->name() <<"'s turn.  Board for "<< p2->name() <<":"<<endl;
        //cout the display of the other player's board before any attack
        b2.display(p1->isHuman()); //if human, only show shots - seeing ships will be cheating
        
        Point attack = p1->recommendAttack();
        if(b2.attack(attack, shotHit, shipDestroyed, shipID))//attack based on player type
        {
            cout << p1->name() <<" attacked ("<<attack.r<<","<<attack.c<<") and ";
            
            if (shotHit) //if shot hit something, cout that a hit occured
            {
                if (shipDestroyed) //if it destoyed a ship, cout that information
                    cout << "destroyed the "<< p1->game().shipName(shipID) <<", resulting in:"<<endl;
                else
                    cout << "hit something, resulting in:"<<endl;
            }
            else
            {
                cout << "missed, resulting in:" <<endl; //else it missed, cout there was a miss
            }
            
            b2.display(p1->isHuman()); //display result board after attack
            if (b2.allShipsDestroyed())
            {
                cout << p1->name() << " wins!"<<endl; //after each turn, check if all ships have been destroyed
                return p1;
            }
            p1->recordAttackResult(attack, true, shotHit, shipDestroyed, shipID);
        }
        else //if an attack landed where one was already placed, or attack is outside board...
        {
            cout << p1->name() <<" wasted an attack at ("<<attack.r<<","<<attack.c<<").";
            p1->recordAttackResult(attack, false, shotHit, shipDestroyed, shipID);
        }
        
        p2->recordAttackByOpponent(attack);
        
        if (shouldPause)
        {
            cout<<"Press enter to continue: "; //if pauses are wanted, there will be a pause
            string yea; // after every attack
            getline(cin, yea);
        }
        
        cout << p2->name() <<"'s turn.  Board for "<< p1->name() <<":"<<endl;
        //cout the display of the other player's board before any attack
        b1.display(p2->isHuman()); //if human, only show shots - seeing ships will be cheating
        
        attack = p2->recommendAttack();
        if(b1.attack(attack, shotHit, shipDestroyed, shipID))//attack based on player type
        {
            cout << p2->name() <<" attacked ("<<attack.r<<","<<attack.c<<") and ";
            
            if (shotHit)
            {
                if (shipDestroyed)
                    cout << "destroyed the "<< p2->game().shipName(shipID) <<", resulting in:"<<endl;
                else
                    cout << "hit something, resulting in:"<<endl;
            }
            else
            {
                cout << "missed, resulting in:" <<endl;
            }
            
            b1.display(p2->isHuman());
            if (b1.allShipsDestroyed())
            {
                cout << p2->name() << " wins!"<<endl;
                return p2;
            }
            p2->recordAttackResult(attack, true, shotHit, shipDestroyed, shipID);
        }
        else
        {
            cout << p2->name() <<" wasted a shot at ("<<attack.r<<","<<attack.c<<")."<<endl;
            p2->recordAttackResult(attack, false, shotHit, shipDestroyed, shipID);
        }
        
        p1->recordAttackByOpponent(attack);
        
        if (shouldPause)
        {
            cout<<"Press enter to continue: "; //if pauses are wanted, there will be a pause
            string yea; // after every attack
            getline(cin, yea);
        }
    }
}

//******************** Game functions *******************************

// These functions for the most part simply delegate to GameImpl's functions.
// You probably don't want to change any of the code from this point down.

Game::Game(int nRows, int nCols)
{
    if (nRows < 1  ||  nRows > MAXROWS)
    {
        cout << "Number of rows must be >= 1 and <= " << MAXROWS << endl;
        exit(1);
    }
    if (nCols < 1  ||  nCols > MAXCOLS)
    {
        cout << "Number of columns must be >= 1 and <= " << MAXCOLS << endl;
        exit(1);
    }
    m_impl = new GameImpl(nRows, nCols);
}

Game::~Game()
{
    delete m_impl;
}

int Game::rows() const
{
    return m_impl->rows();
}

int Game::cols() const
{
    return m_impl->cols();
}

bool Game::isValid(Point p) const
{
    return m_impl->isValid(p);
}

Point Game::randomPoint() const
{
    return m_impl->randomPoint();
}

bool Game::addShip(int length, char symbol, string name)
{
    if (length < 1)
    {
        cout << "Bad ship length " << length << "; it must be >= 1" << endl;
        return false;
    }
    if (length > rows()  &&  length > cols())
    {
        cout << "Bad ship length " << length << "; it won't fit on the board"
             << endl;
        return false;
    }
    if (!isascii(symbol)  ||  !isprint(symbol))
    {
        cout << "Unprintable character with decimal value " << symbol
             << " must not be used as a ship symbol" << endl;
        return false;
    }
    if (symbol == 'X'  ||  symbol == '.'  ||  symbol == 'o')
    {
        cout << "Character " << symbol << " must not be used as a ship symbol"
             << endl;
        return false;
    }
    int totalOfLengths = 0;
    for (int s = 0; s < nShips(); s++)
    {
        totalOfLengths += shipLength(s);
        if (shipSymbol(s) == symbol)
        {
            cout << "Ship symbol " << symbol
                 << " must not be used for more than one ship" << endl;
            return false;
        }
    }
    if (totalOfLengths + length > rows() * cols())
    {
        cout << "Board is too small to fit all ships" << endl;
        return false;
    }
    return m_impl->addShip(length, symbol, name);
}

int Game::nShips() const
{
    return m_impl->nShips();
}

int Game::shipLength(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipLength(shipId);
}

char Game::shipSymbol(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipSymbol(shipId);
}

string Game::shipName(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipName(shipId);
}

Player* Game::play(Player* p1, Player* p2, bool shouldPause)
{
    if (p1 == nullptr  ||  p2 == nullptr  ||  nShips() == 0)
        return nullptr;
    Board b1(*this);
    Board b2(*this);
    return m_impl->play(p1, p2, b1, b2, shouldPause);
}

