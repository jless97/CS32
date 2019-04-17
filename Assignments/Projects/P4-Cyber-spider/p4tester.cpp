/*#include "IntelWeb.h"
#include "InteractionTuple.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <cstdlib>
using namespace std;

bool getLinesFromFile(string filename, vector<string>& data)
{
    ifstream inf(filename);
    if (!inf)
        return false;
    string line;
    while (getline(inf, line))
        data.push_back(line);
    return true;
}

bool createDB(string databasePrefix, unsigned int expectedMaxNumberOfItems)
{
    IntelWeb iw;
    if ( ! iw.createNew(databasePrefix, expectedMaxNumberOfItems))
    {
        cout << "Error: Cannot create database with prefix " << databasePrefix
        << " with " << expectedMaxNumberOfItems << " items expected." << endl;
    }
    return true;
}

bool ingest(string databasePrefix, string telemetryLogFile)
{
    IntelWeb iw;
    if ( ! iw.openExisting(databasePrefix))
    {
        cout << "Error: Cannot open existing database with prefix " << databasePrefix << endl;
        return false;
    }
    if ( ! iw.ingest(telemetryLogFile))
    {
        cout << "Error: Ingesting telemetry data from " << telemetryLogFile << " failed." << endl;
        return false;
    }
    return true;
}*/