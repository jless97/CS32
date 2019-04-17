//IntelWeb.cpp

#include "IntelWeb.h"
#include "DiskMultiMap.h"
#include "InteractionTuple.h"
#include "BinaryFile.h"
#include <iostream>
#include <fstream>  // needed in addition to <iostream> for file I/O
#include <sstream>  // needed in addition to <iostream> for string stream I/O
#include <string>
#include <set>
#include <vector>

const std::string forwardMap = "forwardMap.dat";
const std::string reverseMap = "reverseMap.dat";

IntelWeb::IntelWeb()
{}

IntelWeb::~IntelWeb()
{
    close();
}

bool IntelWeb::createNew(const std::string& filePrefix, unsigned int maxDataItems)
{
    close();
    int numBuckets = maxDataItems / 0.70;
    bool success = true;
    if (! m_forwardTable.createNew(filePrefix + forwardMap, numBuckets))
        success = false;
    if (! m_reverseTable.createNew(filePrefix + reverseMap, numBuckets) && success == true)
    {
        success = false;
        m_forwardTable.close();
    }
    return success;
}

bool IntelWeb::openExisting(const std::string& filePrefix)
{
    close();
    bool success = true;
    if (! m_forwardTable.openExisting(filePrefix + forwardMap))
        success = false;
    if (! m_reverseTable.openExisting(filePrefix + reverseMap) && success == true)
    {
        success = false;
        m_forwardTable.close();
    }
    return success;
}

void IntelWeb::close()
{
    m_forwardTable.close();
    m_reverseTable.close();
}

bool IntelWeb::ingest(const std::string& telemetryFile)
{
    ifstream inf(telemetryFile);
		  // Test for failure to open
    if ( ! inf)
    {
        cerr << "Cannot open telemetryFile!" << endl;
        return false;
    }

    string line;
    while (getline(inf, line))
    {
        istringstream iss(line);
        string from, to, context;
        if ( ! (iss >> context >> from >> to))
        {
            cerr << "Ignoring badly-formatted input line: " << line << endl;
            continue;
        }
        cerr << "From: " << from << " " << "To: " << to << " " << "Context: " << context << endl;
        
        // Add data to DiskMultiMap maps
        if (! m_forwardTable.insert(from, to, context) || ! m_reverseTable.insert(to, from, context))
            return false;
    }
    return true;
}

unsigned int IntelWeb::crawl(const std::vector<std::string>& indicators, unsigned int minPrevalenceToBeGood,
                std::vector<std::string>& badEntitiesFound, std::vector<InteractionTuple>& badInteractions)
{
    unsigned int maliciousEntitiesCounter = 0;
    /*vector<std::string> maliciousEntitiesIndicators;
    set<std::string> sortedMaliciousEntitiesIndicators;
    for (int i = 0; i < indicators.size(); i++)
    {
        maliciousEntitiesIndicators.push_back(indicators[i]);
    }
    for (int i = 0; i < maliciousEntitiesIndicators.size(); i++)
    {
        DiskMultiMap::Iterator it = m_forwardTable.search(maliciousEntitiesIndicators[i]);
        int currentMaliciousEntityCounter = 0;
        while (it.isValid())
        {
            currentMaliciousEntityCounter++;
            ++it;
        }
        it = m_reverseTable.search(maliciousEntitiesIndicators[i]);
        while (it.isValid())
        {
            currentMaliciousEntityCounter++;
            ++it;
        }
        if (currentMaliciousEntityCounter < minPrevalenceToBeGood)
        {
            sortedMaliciousEntitiesIndicators.insert(maliciousEntitiesIndicators[i]);
            maliciousEntitiesCounter++;
        }
        
        it = m_reverseTable.search(maliciousEntitiesIndicators[i]);
        while (it.isValid())
        {
            currentMaliciousEntityCounter++;
            ++it;
        }
        it = m_forwardTable.search(maliciousEntitiesIndicators[i]);
        while (it.isValid())
        {
            currentMaliciousEntityCounter++;
            ++it;
        }
        if (currentMaliciousEntityCounter < minPrevalenceToBeGood)
        {
            sortedMaliciousEntitiesIndicators.insert(maliciousEntitiesIndicators[i]);
            maliciousEntitiesCounter++;
        }
    }
    for (set<std::string>::iterator it = sortedMaliciousEntitiesIndicators.begin();
         it != sortedMaliciousEntitiesIndicators.end(); it++)
    {
        badEntitiesFound.push_back(*it);
    }
    */
    return maliciousEntitiesCounter;
}

bool IntelWeb::purge(const std::string& entity)
{
    bool anyRemoved = false;
    int anyRemovedCounter = 0;
    DiskMultiMap::Iterator it = m_forwardTable.search(entity); //Search where entity would be a key
        
    while (it.isValid()) //While there are still nodes left in the list at the given bucket
    {
        anyRemovedCounter = m_forwardTable.erase((*it).key, (*it).value, (*it).context);
        m_reverseTable.erase((*it).value, (*it).key, (*it).context);
        
        if (anyRemovedCounter > 0)
            anyRemoved = true;
        
        ++it;
    }

    it = m_reverseTable.search(entity); //Search where entity would be a value
    while (it.isValid()) //While there are still nodes left in the list at the given bucket
    {
        anyRemovedCounter = m_reverseTable.erase((*it).key, (*it).value, (*it).context);
        m_forwardTable.erase((*it).value, (*it).key, (*it).context);
        
        if (anyRemovedCounter > 0)
            anyRemoved = true;
        
        ++it;
    }
    
    return anyRemoved;
}







