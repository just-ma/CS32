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
    if (sr == er && sc == ec)
        return true;
    maze[sr][sc] = 'X';
    if (maze[sr-1][sc] != 'X')
    {
        if (pathExists(maze, sr-1, sc, er, ec))
            return true;
    }
    if (maze[sr][sc+1] != 'X')
    {
        if (pathExists(maze, sr, sc+1, er, ec))
            return true;
    }
    if (maze[sr+1][sc] != 'X')
    {
        if (pathExists(maze, sr+1, sc, er, ec))
            return true;
    }
    if (maze[sr][sc-1] != 'X')
    {
        if (pathExists(maze, sr, sc-1, er, ec))
            return true;
    }
    return false;
}
