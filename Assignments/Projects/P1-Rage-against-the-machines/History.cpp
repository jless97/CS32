#include "History.h"
#include "globals.h"

#include <iostream>
#include <cstdlib> // ==> Didn't include
using namespace std;

History::History (int nRows, int nCols)
{
    m_rows = nRows;
    m_cols = nCols;
    int r, c;
    for (r = 1; r <= m_rows; r++)
        for (c = 1; c <= m_cols; c++)
            m_history[r][c] = 0;
}

bool History::record(int r, int c)
{
    if (r <= 0 || r > m_rows || c <= 0 || c > m_cols)
        return false;
    m_history[r][c]++;
    return true;
    
}

void History::display() const
{
    char historyGrid[MAXROWS][MAXCOLS];
    int r, c;
    for (r = 1; r <= m_rows; r++)
        for (c = 1; c <= m_cols; c++)
        {
            int counter = m_history[r][c];
            if (counter == 0)
                historyGrid[r - 1][c - 1] = '.';
            else if (counter > 0 && counter < 26)
                historyGrid[r - 1][c - 1] = 'A' + counter - 1;
            else
                historyGrid[r - 1][c - 1] = 'Z';
        }
    
    clearScreen();
    for (r = 1; r <= m_rows; r++)
    {
        for (c = 1; c <= m_cols; c++)
            cout << historyGrid[r - 1][c - 1];
        cout << endl;
    }
    cout << endl;
}