#include "Player.h"
#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>
#include <string>

using namespace std;

//*********************************************************************
//  AwfulPlayer
//*********************************************************************

class AwfulPlayer : public Player
{
  public:
    AwfulPlayer(string nm, const Game& g);
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                                bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
  private:
    Point m_lastCellAttacked;
};

AwfulPlayer::AwfulPlayer(string nm, const Game& g)
 : Player(nm, g), m_lastCellAttacked(0, 0)
{}

bool AwfulPlayer::placeShips(Board& b)
{
      // Clustering ships is bad strategy
    for (int k = 0; k < game().nShips(); k++)
        if ( ! b.placeShip(Point(k,0), k, HORIZONTAL))
            return false;
    return true;
}

Point AwfulPlayer::recommendAttack()
{
    if (m_lastCellAttacked.c > 0)
        m_lastCellAttacked.c--;
    else
    {
        m_lastCellAttacked.c = game().cols() - 1;
        if (m_lastCellAttacked.r > 0)
            m_lastCellAttacked.r--;
        else
            m_lastCellAttacked.r = game().rows() - 1;
    }
    return m_lastCellAttacked;
}

void AwfulPlayer::recordAttackResult(Point /* p */, bool /* validShot */,
                                     bool /* shotHit */, bool /* shipDestroyed */,
                                     int /* shipId */)
{
      // AwfulPlayer completely ignores the result of any attack
}

void AwfulPlayer::recordAttackByOpponent(Point /* p */)
{
      // AwfulPlayer completely ignores what the opponent does
}

//*********************************************************************
//  HumanPlayer
//*********************************************************************

bool getLineWithTwoIntegers(int& r, int& c)
{
    bool result(cin >> r >> c);
    if (!result)
        cin.clear();  // clear error state so can do more input operations
    cin.ignore(10000, '\n');
    return result;
}

class HumanPlayer : public Player
{
public:
    HumanPlayer(string nm, const Game& g);
    virtual bool isHuman() const {return true;} //automatically returns true for isHuman function
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit, //this function does nothing
                                    bool shipDestroyed, int shipId) {}
    virtual void recordAttackByOpponent(Point p) {} //this function also does nothing
};

HumanPlayer::HumanPlayer(string nm, const Game& g) //initializes only the player name and game
: Player(nm, g) {}

bool HumanPlayer::placeShips(Board& b)
{
    int r, c;
    string dir0 = "";
    Direction dir;
    string input;
    
    for (int n = 0; n < game().nShips(); n++) //for each ship that needs to be placed.
    {
        b.display(0);
        while (true) //loops forever until break
        {
            cout<<"Enter h or v for direction of " << game().shipName(n) <<
                " (length " << game().shipLength(n)<<"): "; //cout input message
            getline( cin , dir0 );
            
            if (dir0[0] != 'v' && dir0[0] != 'h')
                                //unless the input is 'h' or 'v', the loop will
                cout<<"Direction must be h or v."<<endl;  // continue to loop
            else
                break;
        }
        if (dir0[0] == 'h')
            dir = HORIZONTAL; //assign direction based on whether input was v or h
        else
            dir = VERTICAL;
        
        while (true) // another infinite loop to find the coordinate
        {
            cout<<"Enter row and column of topmost cell (e.g. 3 5): "; //input message
            
            if(getLineWithTwoIntegers(r, c))
            {
                if (b.placeShip(Point(r, c), n, dir))
                    break; //if you get two numbers, and the placeship function works, break from loop
                else
                    cout<<"The ship can not be placed there."<<endl; // if not, keep going
            }
            else
                cout<<"You must enter two integers."<<endl;
        }
    }
    return true;
}

Point HumanPlayer::recommendAttack()
{
    int r,c;
    while (true) //infinite loop; keeps loop until it gets two valid ints
    {
        cout<<"Enter the row and column to attack (e.g, 3 5): "; //asks for row and column of ship
        if (getLineWithTwoIntegers(r, c))    //uses getLineWithTwoIntegers function
        {
            Point p(r,c);                    //creates point from those two integers
            return p;                           //returns point
        }
        else
            cout<<"You must enter two integers."<<endl; //if input is invalid
    }
}


//*********************************************************************
//  MediocrePlayer
//*********************************************************************

class MediocrePlayer : public Player
{
public:
    MediocrePlayer(string nm, const Game& g);
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                    bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
private:
    Point core; //core is used when a hit has been made. core becomes center of cross
    bool helper(int ID, Board& b); //recursive function for placing ships
    int array[100];
    int state = 1, hunt = 0;
    int board[10][10]; //board that records all hits and misses
};

MediocrePlayer::MediocrePlayer(string nm, const Game& g)
: Player(nm, g)
{
    for (int r = 0; r < 10; r++)
    {
        for (int c = 0; c < 10; c++)
        {
            board[r][c] = 0; //initializes all points to 0
        }
    }
}

bool MediocrePlayer::helper(int ID, Board& b) //recursive function
{
    if (ID == game().nShips()) //base condition - for ship out of bounds
        return true;
    for ( int r = 0; r < game().rows(); r++)
    {
        for ( int c = 0; c < game().cols(); c++)
        {
            if (b.placeShip( Point(r,c), array[ID], HORIZONTAL)) //if able to place current ship horizontally
            {
                if(helper(ID + 1, b)) //see if the magic (recursive) function works too
                    return true;
                else
                    b.unplaceShip(Point(r,c), array[ID], HORIZONTAL); //else unplace current ship
            }
            if (b.placeShip( Point(r,c), array[ID], VERTICAL)) //if able to place current ship vertically
            {
                if(helper(ID + 1, b)) //see if the magic (recursive) function works too
                    return true;
                else
                    b.unplaceShip(Point(r,c), array[ID], VERTICAL); //else unplace current ship
            }
        }
    }
    return false; // if all points don't work, return false
}

bool MediocrePlayer::placeShips(Board& b)
{
    int count = 0;
    for (int m = 10; m > 0; m--) //this orders all ships in an array from largest to smallest
    {
        for (int n = 0; n < game().nShips(); n++) // for when placing, so that the process goes faster
        {
            if (game().shipLength(n) == m)
            {
                array[count] = n;
                count ++;
            }
        }
    }
    for (int n = 0; n < 50; n++) //for 50 times..
    {
        b.block();
        if (helper(0, b)) //try the recursive function until it works
        {
            b.unblock();
            return true; //if it works, unblock current blocks and return true
        }
        b.unblock(); //else unblock and try again
    }
    return false; //else return false, if unable to place ships after 50 tries
}

Point MediocrePlayer::recommendAttack()
{
    int row, col;
    if (state == 1) //the first state looks for a random point within the board that hasn't been ht yet
    {
        while (true)
        {
            row = randInt(game().rows());
            col = randInt(game().cols());
            if (board[row][col] == 0) //until true, it keeps looking
                break;
        }
    }
    else //else if in state 2, it attacks in a cross.
    {
        while (true) //repeatedly
        {
            if(randInt(2) == 0) //if true, looks for a point in the column of the cross
            {
                row = core.r + randInt(9) - 4;
                col = core.c;
            }
            else //else looks for a point in the row of the cross
            {
                row = core.r;
                col = core.c + randInt(9) - 4;
            }
            if (row < 0 || col < 0 || row >= game().rows() || col >= game().cols())
                continue;
            if (board[row][col] == 0) //if that point hasn't been attacked yet, return that point
            {
                hunt --; //number of available points decreases by 1
                break;
            }
        }
    }
    board[row][col] = 1; //record that this point has been shot at
    return Point(row, col);
}

void MediocrePlayer::recordAttackResult(Point p, bool validShot, bool shotHit,
                                        bool shipDestroyed, int shipId)
{
    if (shotHit) //if shot hit something
    {
        if (shipDestroyed) //if shot destroyed something
        {
            state = 1; //return to state 1 and keep looking for random points
            return;
        }
        if (state == 1) //if shot didn't destroy anything, and currently in state 1
        {
            core = p; //make this point the core
            hunt = 0;
            for (int n = 0; n < 9; n++) //determine the number of valid points on the cross
            {
                if (game().isValid(Point(core.r-4+n, core.c)) && board[core.r-4+n][core.c] == 0)
                    hunt ++;
                if (game().isValid(Point(core.r, core.c-4+n)) && board[core.r][core.c-4+n] == 0)
                    hunt ++;
            }
            state = 2; //make state equal to 2
        }
        if (hunt == 0) //if all valid points in the cross have been attacked
        {
            state = 1; //return to state 1
        }
    }
}

void MediocrePlayer::recordAttackByOpponent(Point p) {}

//*********************************************************************
//  GoodPlayer
//*********************************************************************



class GoodPlayer : public Player
{
public:
    GoodPlayer(string nm, const Game& g);
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                    bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
private:
    Point ep1, ep2; //point that represents the endpoints of the ship
    bool helper(int ID, Board& b); //recursive function to help place ships
    int array[100];
    int state = 1; //states of recommendattack
    int board[10][10]; //board for referencing
    bool end1 = false, end2 = false, weird = false, lowestis2 = true;
    //these bools help determine if either ends have been reached
    int turn = 0;
    Direction dir; //direction of ship
};

GoodPlayer::GoodPlayer(string nm, const Game& g)
: Player(nm, g)
{
    for (int r = 0; r < 10; r++)
    {
        for (int c = 0; c < 10; c++)
        {
            board[r][c] = 0; //initializes board to have all 0's
        }
    }
}

bool GoodPlayer::helper(int ID, Board& b) //recursive function
{
    if (ID == game().nShips()) //base condition - for ship out of bounds
        return true;
    for ( int r = 0; r < game().rows(); r++)
    {
        for ( int c = 0; c < game().cols(); c++)
        {
            if (b.placeShip( Point(r,c), array[ID], HORIZONTAL)) //if able to place current ship horizontally
            {
                if(helper(ID + 1, b)) //see if the magic (recursive) function works too
                    return true;
                else
                    b.unplaceShip(Point(r,c), array[ID], HORIZONTAL); //else unplace current ship
            }
            if (b.placeShip( Point(r,c), array[ID], VERTICAL)) //if able to place current ship vertically
            {
                if(helper(ID + 1, b)) //see if the magic (recursive) function works too
                    return true;
                else
                    b.unplaceShip(Point(r,c), array[ID], VERTICAL); //else unplace current ship
            }
        }
    }
    return false; // if all points don't work, return false
}

bool GoodPlayer::placeShips(Board& b)
{
    int count = 0;
    for (int m = 10; m > 0; m--) //this orders all ships in an array from largest to smallest
    {
        for (int n = 0; n < game().nShips(); n++) // for when placing, so that the process goes faster
        {
            if (game().shipLength(n) == m)
            {
                array[count] = n;
                count ++;
            }
        }
    }
    for (int n = 0; n < 50; n++) //for 50 times..
    {
        b.block();
        if (helper(0, b)) //try the recursive function until it works
        {
            b.unblock();
            return true; //if it works, unblock current blocks and return true
        }
        b.unblock(); //else unblock and try again
    }
    return false; //else return false, if unable to place ships after 50 tries
}


Point GoodPlayer::recommendAttack()
{
    int row = game().rows()/2, col = game().cols()/2; //initailizes point to somewhere in the middle
    
    if (state == 1) //if state = 1...
    {
        int ticks = 0; //ticks make sure the loop doesn't go forever
        turn ++;
        if (turn < 15) //while less than 15 turns
        {
            while (board[row][col] != 0)
            {
                ticks ++;
                if(lowestis2) //only if the smallest ship is greater than 2
                {
                    if (randInt(2)) //50 percent chance of this option
                    {
                        row = randInt(game().rows()/4)*2 + 1; //checkerboard pattern
                        col = randInt(game().cols()/2+game().cols()%2)*2;
                    }
                    else
                    {
                        row = randInt(game().rows()/4 + game().rows()%2)*2;
                        col = randInt(game().cols()/2)*2 + 1; //also checkerboard pattern
                    }
                }
                else //if the smallest ship is 1, search for ships without checkerboard pattern
                {
                    row = randInt(game().rows()/2);
                    col = randInt(game().cols());
                }
                if (ticks == 50)
                {
                    turn = 20;
                    break;
                }
            }
        }
        else //after 15 turns in the top half of the board...
        {
            while (board[row][col] != 0) // switch to attacking entire board
            {
                ticks ++;
                if (lowestis2) // if the smallest ship is greater than 1
                {
                    if (randInt(2)) //use checkerboard pattern
                    {
                        row = randInt(game().rows()/2)*2 + 1;
                        col = randInt(game().cols()/2+game().cols()%2)*2;
                    }
                    else
                    {
                        row = randInt(game().rows()/2 + game().rows()%2)*2;
                        col = randInt(game().cols()/2)*2 + 1;
                    }
                }
                else //if the smallest ship is actually 1
                {
                    row = randInt(game().rows()); //don't use checkerboard pattern
                    col = randInt(game().cols());
                }
                if (ticks == 50)
                {
                    row = randInt(game().rows());
                    col = randInt(game().cols());
                    break;
                }
            }
        }
        return Point(row,col);
    }
    else if (state == 3) //STATE 3
    {
        if (dir == HORIZONTAL)
        {
            
            // checks if the first endpoint has been reached yet. Then checks if the point further is
            // valid. Then checks if the point has not been attacked yet. If all those conditions
            // are true, return that point
            
            if (!end1 && game().isValid(Point(ep1.r, ep1.c - 1)) && board[ep1.r][ep1.c - 1] == 0)
                return Point(ep1.r, ep1.c - 1);
            else
                end1 = true;
            if (!end2 && game().isValid(Point(ep2.r, ep2.c + 1)) && board[ep2.r][ep2.c + 1] == 0)
                return Point(ep2.r, ep2.c + 1);
            else
                end2 = true;
        }
        else if (dir == VERTICAL)
        {
            
            // Same as above but for vertical case
            // checks if the first endpoint has been reached yet. Then checks if the point further is
            // valid. Then checks if the point has not been attacked yet. If all those conditions
            // are true, return that point
            
            if (!end1 && game().isValid(Point(ep1.r - 1, ep1.c)) && board[ep1.r - 1][ep1.c] == 0)
                return Point(ep1.r - 1, ep1.c);
            else
                end1 = true;
            if (!end2 && game().isValid(Point(ep2.r + 1, ep2.c)) && board[ep2.r + 1][ep2.c] == 0)
                return Point(ep2.r + 1, ep2.c);
            else
                end2 = true;
        }
        weird = true; //if both ends have been reached, weird condition is true
        state = 2;
    }
    
    if (state == 2)
    {
        if (weird) //if in weird condition
        {
            for(int r = 0; r < game().rows(); r++)
            {
                for (int c = 0; c < game().cols(); c++)
                {
                    if (board[r][c] == 2) //look through grid for a point that has a hit but not sunk
                    {
                        ep1 = Point(r,c); //make that point the center point
                        ep2 = Point(r,c);
                    }
                }
            }
        }
        for (int n = 1; n < 5; n++) //takes in both weird and nonweird conditions
        {
            if (game().isValid(Point(ep1.r-n, ep1.c)) && board[ep1.r - n][ep1.c] == 0) //looks down
                return Point(ep1.r - n, ep1.c);
            else if (game().isValid(Point(ep1.r, ep1.c-n)) && board[ep1.r][ep1.c - n] == 0) //left
                return Point(ep1.r, ep1.c - n);
            else if (game().isValid(Point(ep1.r+n, ep1.c)) && board[ep2.r + n][ep2.c] == 0) //up
                return Point(ep2.r + n, ep2.c);
            else if (game().isValid(Point(ep1.r, ep1.c+n)) && board[ep2.r][ep2.c + n] == 0) //right
                return Point(ep2.r, ep2.c + n);
        } //increases span each time until a hit
        
        weird = true; //if no hits, weird is true, and find a new point that has been hit
        return Point(randInt(game().rows()), randInt(game().cols()));
    }
    return Point(row,col);
}

void GoodPlayer::recordAttackResult(Point p, bool validShot, bool shotHit,
                                    bool shipDestroyed, int shipId)
{
    if (!validShot) //if shot is invalid, return
        return;
    
    if (shotHit) //if shot hit something
    {
        if (shipDestroyed) //hit and destroyed
        {
            if (p.r != ep1.r)
                dir = VERTICAL; //determine the direction of the ship
            else
                dir = HORIZONTAL;
            
            if (dir == HORIZONTAL) //records the sinking ship on the 2D array
            {
                if (p.c < ep1.c)
                {
                    for (int n = 0; n < game().shipLength(shipId); n++)
                        board[p.r][p.c + n] = 3; //turns all points to 3 which represents sunk
                }
                else
                {
                    for (int n = 0; n < game().shipLength(shipId); n++)
                        board[p.r][p.c - n] = 3; //turns all points to 3 which represents sunk
                }
            }
            if (dir == VERTICAL) //records sinking ship vertical case
            {
                if (p.r < ep1.r)
                {
                    for (int n = 0; n < game().shipLength(shipId); n++)
                        board[p.r + n][p.c] = 3; //turns all points to 3 which represents sunk
                }
                else
                {
                    for (int n = 0; n < game().shipLength(shipId); n++)
                        board[p.r - n][p.c] = 3; //turns all points to 3 which represents sunk
                }
            }
            
            if (weird) //if still in weird condition
            {
                bool yes (true);
                for(int r = 0; r < game().rows(); r++)
                {
                    for (int c = 0; c < game().cols(); c++) //checks if still weird by looking through array
                    {
                        if (board[r][c] == 2) //if there is still a spot that has an unsunken part, still weird
                        {
                            yes = false;
                        }
                    }
                }
                if (yes)
                    weird = false;
            }
            
            end1 = false; //resent ends
            end2 = false;
            
            if(weird) //if weird, return to state 2
            {
                state = 2;
            }
            else //if out of weird condition, return to state 1 and go back to looking for ships
            {
                state = 1;
            }
            
        }
        else //hit but didnt destory
        {
            board[p.r][p.c] = 2; //mark position as 2 - hit but not sunken
            
            if (state == 1) //if it was seeking ship
            {
                state = 2; //turn state to find direction of ship
                ep1 = p;
                ep2 = p;
            }
            else if (state == 2) //if it was trying to find the direction of the ship
            {
                if (p.r < ep1.r) //determine whether it is vertical or horizontal
                {
                    ep1.r = p.r;
                    dir = VERTICAL;
                }
                else if (p.r > ep1.r)
                {
                    ep2.r = p.r;
                    dir = VERTICAL;
                }
                else if (p.c < ep1.c)
                {
                    ep1.c = p.c;
                    dir = HORIZONTAL;
                }
                else
                {
                    ep2.c = p.c;
                    dir = HORIZONTAL;
                }
                state = 3; //in any case, state is now 3; now trying to finish off the ship
                return;
            }
            if (state == 3) //in state 3
            {
                if (dir == HORIZONTAL) //horizontal case for determining whether the end has been reached
                {
                    if (ep1.c == 0 || board[ep1.r][ep1.c - 1] == 1)
                        end1 = true;
                    if (ep2.c == game().cols() - 1 || board[ep2.r][ep2.c + 1] == 1)
                        end2 = true;
                }
                else if (dir == VERTICAL) //vertical case for determining whether the end has been reached
                {
                    if (ep1.r == 0 || board[ep1.r - 1][ep1.c] == 1)
                        end1 = true;
                    if (ep2.r == game().rows() - 1 || board[ep2.r + 1][ep2.c] == 1)
                        end2 = true;
                }
                if (!end1) //if first end has not been reached, update the first point
                {
                    ep1 = p;
                }
                else //if the first point has been found, update the second point
                {
                    ep2 = p;
                }
                if (end1 && end2) //if both ends have been found and no ship has been sunk...
                {
                    weird = true; //weird is activated
                    state = 2; //return to state 2
                    return;
                }
            }
        }
    }
    else //missed
    {
        board[p.r][p.c] = 1; //if missed, record that on the 2D array
    }
}

void GoodPlayer::recordAttackByOpponent(Point p){}


//*********************************************************************
//  createPlayer
//*********************************************************************

Player* createPlayer(string type, string nm, const Game& g)
{
    static string types[] = {
        "human", "awful", "mediocre", "good"
    };
    
    int pos;
    for (pos = 0; pos != sizeof(types)/sizeof(types[0])  &&
                                                     type != types[pos]; pos++)
        ;
    switch (pos)
    {
      case 0:  return new HumanPlayer(nm, g);
      case 1:  return new AwfulPlayer(nm, g);
      case 2:  return new MediocrePlayer(nm, g);
      case 3:  return new GoodPlayer(nm, g);
      default: return nullptr;
    }
}
