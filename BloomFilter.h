#ifndef BLOOMFILTER_H
#define BLOOMFILTER_H

#include <string>
#include <vector>

using namespace std;

// Bloom Filter class
class BloomFilter
{
private:
    vector<bool> bitArray; // array to store bits
    int size;              // size of bit array
    int numHashFunctions;  // number of hash functions

    // hash function 1
    unsigned int hash1(const string &str) const;
    // hash function 2
    unsigned int hash2(const string &str) const;
    // hash function 3
    unsigned int hash3(const string &str) const;

public:
    // constructor
    BloomFilter(int filterSize = 10000, int numHashes = 3);

    // insert item into filter
    void insert(const string &item);

    // check if item might be in filter
    bool mightContain(const string &item) const;

    // clear the filter
    void clear();
};

#endif
