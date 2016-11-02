#define _CRT_SECURE_NO_WARNINGS
#include "DiskMultiMap.h"
#include "BinaryFile.h"
#include "MultiMapTuple.h"

#include <string>
#include <cstring>
#include <functional>
using namespace std;

const int NULLPTR = 0; // For conceptual purposes based off of Smallberg's implementation of DiskList

DiskMultiMap::Iterator::Iterator()
{
    m_isValid = false;
}

DiskMultiMap::Iterator::Iterator(BinaryFile* filename, const std::string& key, BinaryFile::Offset bucketOffset)
{
    m_currentFile = filename;
    m_currentFile->read(m_currentNode, bucketOffset);
    m_isValid = true;
}

bool DiskMultiMap::Iterator::isValid() const
{
    return m_isValid;
}

DiskMultiMap::Iterator& DiskMultiMap::Iterator::operator++()
{
    if (m_currentNode.next == NULLPTR)
    {
        m_isValid = false;
        return *this;
    }
    else
    {
        BinaryFile::Offset currentNodeOffset = m_currentNode.next;
        m_currentFile->read(m_currentNode, currentNodeOffset);
    }
    return *this;
}

MultiMapTuple DiskMultiMap::Iterator::operator*()
{
    MultiMapTuple tupleNode;
    tupleNode.key = std::string(m_currentNode.m_key);
    tupleNode.value = std::string(m_currentNode.m_value);
    tupleNode.context = std::string(m_currentNode.m_context);
    return tupleNode;
}

DiskMultiMap::DiskMultiMap()
{
    header.m_nBuckets = 0;
    header.freeList = NULLPTR;
}

DiskMultiMap::~DiskMultiMap()
{
    close();
}

bool DiskMultiMap::createNew(const std::string& filename, unsigned int numBuckets)
{
    close();
    if (! bf.createNew(filename))
        return false;
    
    header.m_nBuckets = numBuckets;
    if (! bf.write(header, 0))
        return false;
    
    int i = 0;
    BinaryFile::Offset bucketOffset = bf.fileLength();
    while (i < numBuckets)
    {
        bf.write(NULLPTR, bucketOffset); //set all lists at the given bucket to nullptr
        bucketOffset += sizeof(BinaryFile::Offset);
        i++;
    }
    return true;
}

bool DiskMultiMap::openExisting(const std::string& filename)
{
    if (bf.isOpen())
        bf.close();
    if (! bf.openExisting(filename))
        return false;
    return true;
}

void DiskMultiMap::close()
{
    if (bf.isOpen())
        bf.close();
}

bool DiskMultiMap::insert(const std::string& key, const std::string& value, const std::string& context)
{
    // Thank you Professor Smallberg for providing the format :)

    if (! validInput(key, value, context)) //If the key/value/context aren't the right input size
        return false;
    
    Node p = copyValues(key, value, context); //Create a new node p to insert with the input
    BinaryFile::Offset pOffset = acquireNode(); //Acquire the correct offset (any to reuse?)
    BinaryFile::Offset bucketOffset = hashFunc(key.c_str()); //Locate the correct bucket
    bf.read(p.next, bucketOffset); //Update the p next pointer each time you insert
    bf.write(p, pOffset); //Write into the disk file p at the given offset
    bf.write(pOffset, bucketOffset); //Update the bucket to point to the items in it's list
    return true;
}

DiskMultiMap::Iterator DiskMultiMap::search(const std::string& key)
{
    Node currNode; //Create a node at the given bucket, in order to get the correct offsets
    BinaryFile::Offset curr = NULLPTR; //Create a curr pointer to traverse through the list
    BinaryFile::Offset currNodeOffset = hashFunc(key); //Locate the correct bucket
    
    bf.read(currNode.next, currNodeOffset); //Read in the values into currNode to give to curr
    curr = currNode.next;
    if (curr == NULLPTR) // If there is an empty list at the given bucket
    {
        Iterator it;
        return it;
    }
    else
    {
        Iterator it(&bf, key, curr); //Create an iterator with the 3-parameter of the given location
        return it;
    }
}

int DiskMultiMap::erase(const std::string& key, const std::string& value, const std::string& context)
{
    // Thank you Professor Smallberg for providing the format :)
    
    int anyRemoved = 0; //Keep track of how many nodes were removed with the given key/value/context
    BinaryFile::Offset bucketOffset = hashFunc(key.c_str()); //Locate the correct bucket
    
    Node prevNode; //Create two nodes to traverse through the list at the given bucket number
    BinaryFile::Offset prev = NULLPTR;
    Node currNode;
    bf.read(currNode.next, bucketOffset); //Read in the values into the currNode
    BinaryFile::Offset curr = currNode.next;
    
    while (curr != NULLPTR) //While we are not at the end of the list
    {
        bf.read(currNode, currNode.next); // Advance the current node
        if (strcmp(currNode.m_value, value.c_str()) != 0 ||
            strcmp(currNode.m_context, context.c_str()) !=0) //If we found an incorrect value, context
        {
            prev = curr; //Advance the pointers to the next positions
            curr = currNode.next;
        }
        else //We found a match
        {
            // Unhook node from linked list
            
            Node toBeRemovedNode;
            BinaryFile::Offset toBeRemoved = curr; //Create a pointer that points to the node to remove
            curr = currNode.next; //Advance the curr pointer to one just after the one to remove
            if (prev == NULLPTR) //If the node we are deleting is the first one
                bf.write(curr, bucketOffset); //Then set the curr node as the first node
            else //If the node to delete is not the first one
            {
                bf.read(prevNode, prev);
                prevNode.next = curr; //Relink the nodes around the deleted node
            }
            
            // Add removed node to front of freeList
            
            bf.read(header, 0);
            bf.read(toBeRemovedNode, toBeRemoved); //Read in values into the node to remove
            toBeRemovedNode.next = header.freeList; //Link the removed node into the free list offsets
            header.freeList = toBeRemoved; //Set the free list to point to the new deleted node
            
            bf.write(header, 0);
            bf.write(toBeRemovedNode, toBeRemoved); //Removal of the node to the free list
            
            anyRemoved++; //If a node was deleted, increment count
        }
    }
    return anyRemoved; //Return how many nodes with the given key/value/context were deleted
}

BinaryFile::Offset DiskMultiMap::hashFunc(const std::string& hashMe)
{
    // String hash function: the bucket number = key % number of buckets
    
    std::hash<std::string> str_hash;
    Header header;
    bf.read(header, 0);
    BinaryFile::Offset bucketOffset = str_hash(hashMe) % header.m_nBuckets;
    return bucketOffset + sizeof(BinaryFile::Offset);
}

BinaryFile::Offset DiskMultiMap::acquireNode()
{
    // Thank you Professor Smallberg for providing the format :)
    
    // Helper function: return a node at an available offset for insert function
    // Create new offset in the file only if no old offset is available for reuse
    
    if (header.freeList == NULLPTR) //If there is no old offset to reuse
    {
        BinaryFile::Offset pOffset = bf.fileLength(); //Then create new offset for new node
        return pOffset;
    }
    //There is an old offset that can be reused
    Node p; //Create a new node called p
    BinaryFile::Offset pOffset = header.freeList; //Set new node p's offset to the old node to be reused
    bf.read(p, header.freeList); //Read in the offset values into the new node p
    header.freeList = p.next; //Set the free list offset to the next available offset
    return pOffset; //Return the offset of the node to be inserted
}

Node DiskMultiMap::copyValues(const std::string& key, const std::string& value, const std::string& context)
{
    // Helper Function: copy in the key/value/context tuple into the given node to insert
    Node copyNode;
    strcpy(copyNode.m_key, key.c_str());
    strcpy(copyNode.m_value, value.c_str());
    strcpy(copyNode.m_context, context.c_str());
    return copyNode;
}

bool DiskMultiMap::validInput(const std::string& key, const std::string& value, const std::string& context)
{
    //Helper function: check to make sure the input is of the correct size
    
    if (key.size() > MAX_DATA_LENGTH || value.size() > MAX_DATA_LENGTH || context.size() > MAX_DATA_LENGTH)
        return false;
    return true;
}
