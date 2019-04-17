#include <iostream>
#include <streambuf>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <cassert>
using namespace std;

/* Corrected version for the pathExists function. My call did not work properly because, it would always return true, no matter if the path was solvable or not. This was caused because I would check if there was a '.' in a given direction, and */

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
    if (sr == er && sc == ec)
        return true;
    maze[sr][sc] = '@';
    if (maze[sr - 1][sc] == '.' && pathExists(maze, nRows, nCols, sr - 1, sc, er, ec))
    {
        return true;
    }
    if (maze[sr][sc + 1] == '.' && pathExists(maze, nRows, nCols, sr, sc + 1, er, ec))
    {
        return true;
    }
    if (maze[sr + 1][sc] == '.' && pathExists(maze, nRows, nCols, sr + 1, sc, er, ec))
    {
        return true;
    }
    if (maze[sr][sc - 1] == '.' && pathExists(maze, nRows, nCols, sr, sc - 1, er, ec))
    {
        return true;
    }
    return false;
}

void testone(int n)
{
    static string maze1[10] = {
        { "XXXXXXXXXX" },
        { "X.X..X...X" },
        { "X.XX.X.XXX" },
        { "X....X.X.X" },
        { "XX.X.X...X" },
        { "XXX..X.X.X" },
        { "X...X...XX" },
        { "X.XX..X.XX" },
        { "X....X...X" },
        { "XXXXXXXXXX" },
    };
    
    static string maze2[10] = {
        { "XXXXXXXXXX" },
        { "X.X..X...X" },
        { "X.XX.X.XXX" },
        { "X....X.X.X" },
        { "XX.X.X...X" },
        { "XXX.XX.X.X" },
        { "X...X...XX" },
        { "X.XX..X.XX" },
        { "X....X...X" },
        { "XXXXXXXXXX" },
    };
    
    static string maze3[10] = {
        { "XXXXXXXXXX" },
        { "XX.....XXX" },
        { "X..XX....X" },
        { "X...X...XX" },
        { "X.X.XXX..X" },
        { "XXXX..X..X" },
        { "XX....X..X" },
        { "X.......XX" },
        { "X..XXXXXXX" },
        { "XXXXXXXXXX" },
    };
    
    static string maze4[10] = {
        { "XXXXXXXXXX" },
        { "XX.....XXX" },
        { "X..XX....X" },
        { "X...X...XX" },
        { "X.X.XXX..X" },
        { "XXXX..X..X" },
        { "XX....X..X" },
        { "X.....X.XX" },
        { "X..XXXXXXX" },
        { "XXXXXXXXXX" },
    };

    switch (n)
    {
        default: {
            assert(false);
        } break; case 63: {
            assert(pathExists(maze1, 10, 10, 8, 6, 1, 1));
        } break; case 64: {
            assert(!pathExists(maze2, 10, 10, 8, 6, 1, 1));
        } break; case 65: {
            assert(pathExists(maze3, 10, 10, 4, 3, 7, 1));
        } break; case 66: {
            assert(!pathExists(maze4, 10, 10, 4, 3, 7, 1));
        }
    }
}

int main()
{
    cout << "Enter test number: ";
    int n;
    cin >> n;
    testone(n);
    cout << "Passed" << endl;
}