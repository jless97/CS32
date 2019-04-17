#ifndef DISKMULTIMAP_H_
#define DISKMULTIMAP_H_

#include <string>
#include "MultiMapTuple.h"
#include "BinaryFile.h"

const int MAX_DATA_LENGTH = 120;

struct Node
{
    char m_key[MAX_DATA_LENGTH + 1];
    char m_value[MAX_DATA_LENGTH + 1];
    char m_context[MAX_DATA_LENGTH + 1];
    BinaryFile::Offset next;
};

class DiskMultiMap
{
public:
    
    class Iterator
    {
    public:
        Iterator();
        Iterator(BinaryFile* filename, const std::string& key, BinaryFile::Offset bucketOffset);
        bool isValid() const;
        Iterator& operator++();
        MultiMapTuple operator*();

    private:
        BinaryFile* m_currentFile;
        Node m_currentNode;
        bool m_isValid;
    };
    
    DiskMultiMap();
    ~DiskMultiMap();
    bool createNew(const std::string& filename, unsigned int numBuckets);
    bool openExisting(const std::string& filename);
    void close();
    bool insert(const std::string& key, const std::string& value, const std::string& context);
    Iterator search(const std::string& key);
    int erase(const std::string& key, const std::string& value, const std::string& context);
    
private:
    BinaryFile::Offset acquireNode();
    BinaryFile::Offset hashFunc(const std::string& hashMe);
    Node copyValues(const std::string& key, const std::string& value, const std::string& context);
    bool validInput(const std::string& key, const std::string& value, const std::string& context);
    struct Header
    {
        int m_nBuckets;
        BinaryFile::Offset freeList;
    };
    BinaryFile bf;
    Header header;
};

#endif // DISKMULTIMAP_H_