bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
    if (sr == er && sc == ec)
        return true;
    maze[sr][sc] = '@';
    if (maze[sr - 1][sc] == '.')
    {
        pathExists(maze, nRows, nCols, sr - 1, sc, er, ec);
        return true;
    }
    if(maze[sr][sc + 1] == '.' && pathExists(maze, nRows, nCols, sr, sc + 1, er, ec))
        return true;
    }
    if(maze[sr + 1][sc] == '.' && pathExists(maze, nRows, nCols, sr + 1, sc, er, ec)
        return true;
    }
    if(maze[sr][sc - 1] == '.' && pathExists(maze, nRows, nCols, sr, sc - 1, er, ec))
        return true;
    }
    return false;
}