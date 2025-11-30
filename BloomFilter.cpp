#include "BloomFilter.h"

// constructor - initialize bloom filter
BloomFilter::BloomFilter(int filterSize, int numHashes)
{
    size = filterSize;
    numHashFunctions = numHashes;
    bitArray.resize(size, false); // create bit array with all false
}

// hash function 1
unsigned int BloomFilter::hash1(const string &str) const
{
    unsigned int hash = 5381;
    for (int i = 0; i < str.length(); i++)
    {
        char c = str[i];
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return hash % size;
}

// hash function 2
unsigned int BloomFilter::hash2(const string &str) const
{
    unsigned int hash = 0;
    for (int i = 0; i < str.length(); i++)
    {
        char c = str[i];
        hash = hash * 31 + c;
    }
    return hash % size;
}

// hash function 3
unsigned int BloomFilter::hash3(const string &str) const
{
    unsigned int hash = 0;
    for (int i = 0; i < str.length(); i++)
    {
        char c = str[i];
        hash = hash * 37 + c;
    }
    return hash % size;
}

// insert item into bloom filter
void BloomFilter::insert(const string &item)
{
    // set bits at hash positions to true
    bitArray[hash1(item)] = true;
    bitArray[hash2(item)] = true;
    bitArray[hash3(item)] = true;
}

// check if item might be in bloom filter
bool BloomFilter::mightContain(const string &item) const
{
    // check if all hash positions are true
    if (bitArray[hash1(item)] == true &&
        bitArray[hash2(item)] == true &&
        bitArray[hash3(item)] == true)
    {
        return true;
    }
    return false;
}

// clear all bits in filter
void BloomFilter::clear()
{
    for (int i = 0; i < size; i++)
    {
        bitArray[i] = false;
    }
}
