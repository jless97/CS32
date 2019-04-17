//mazestack.cpp

#include <iostream>
#include <stack>
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

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
    stack<Coord> coordStack;
    coordStack.push(Coord(sr, sc));
    maze[sr][sc] = '@';
    
    while (! coordStack.empty())
    {
        Coord start = coordStack.top();
        int r = start.r();
        int c = start.c();
        coordStack.pop();
        if (r == er && c == ec)
            return true;
        if (maze[r - 1][c] == '.')
        {
            coordStack.push(Coord(r - 1, c));
            maze[r - 1][c] = '@';
        }
        if (maze[r][c + 1] == '.')
        {
            coordStack.push(Coord(r, c + 1));
            maze[r][c + 1] = '@';
        }
        if (maze[r + 1][c] == '.')
        {
            coordStack.push(Coord(r + 1, c));
            maze[r + 1][c] = '@';
        }
        if (maze[r][c - 1] == '.')
        {
            coordStack.push(Coord(r, c - 1));
            maze[r][c - 1] = '@';
        }
    }
    return false;
}