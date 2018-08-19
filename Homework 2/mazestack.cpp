#include<stack>
using namespace std;

class Coord
{
public:
    Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
    int r() const { return m_r; }
    int c() const { return m_c; }
private:
    int m_r;
    int m_c;
};


bool pathExists(char maze[][10], int sr, int sc, int er, int ec)
{
    Coord start(sr,sc);
    stack<Coord> coordStack;
    coordStack.push(start);
    maze[sr][sc] = 'X'; //update starting position to a wall to mark we've already been here
    while (!coordStack.empty())
    {
        int r = coordStack.top().r();
        int c = coordStack.top().c();
        coordStack.pop();
        if (r == er && c == ec)
            return true;
        if (maze[r-1][c] != 'X')
        {
            coordStack.push(Coord(r-1,c));
            maze[r-1][c] = 'X';
        }
        if (maze[r][c+1] != 'X')
        {
            coordStack.push(Coord(r,c+1));
            maze[r][c+1] = 'X';
        }
        if (maze[r+1][c] != 'X')
        {
            coordStack.push(Coord(r+1,c));
            maze[r+1][c] = 'X';
        }
        if (maze[r][c-1] != 'X')
        {
            coordStack.push(Coord(r,c-1));
            maze[r][c-1] = 'X';
        }
    }
    return false;
}
