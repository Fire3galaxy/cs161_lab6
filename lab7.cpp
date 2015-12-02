#include <iostream>
#include <fstream>
#include <vector>
#include <list>

using namespace std;

int log2(int num);

struct block {
    bool valid;
    long long tag;
    
    block() {
        valid = false;
        tag = 0;
    }
};

int main() {
    // Load all addresses into single vector first
    const long TOTAL = 1012176;
    vector<long long> addresses(TOTAL);

    ifstream fs;
    fs.open("trace");

    fs >> hex; // set up hex input
    for (int i = 0; i < TOTAL &&  fs >> addresses.at(i); i++);

    fs.close();


    // Constants
    const int BLOCK_BITS = 4;   // block offset, 16 elements
    const int BLOCK_ELEMS = 16; // for cache size / elem per block
    long long BITMASK[8];       // to get tag for different size
    BITMASK[0] = 0x0000000000000007;
    BITMASK[1] = 0x000000000000000F;
    BITMASK[2] = 0x000000000000001F; 
    BITMASK[3] = 0x000000000000003F; // 6 bits (direct map 1024)
    BITMASK[4] = 0x000000000000007F; // 7 bits (direct map 2048)
    BITMASK[5] = 0x00000000000000FF; // 8 bits (direct map 4096)
    BITMASK[6] = 0x00000000000001FF; // 9 bits (direct map 8192)
    BITMASK[7] = 0x00000000000003FF; // 10 bits (direct map 16384)
    int CACHE_SIZE[5];          // cache size = # of long long in cache
    CACHE_SIZE[0] = 1024;
    CACHE_SIZE[1] = 2048;
    CACHE_SIZE[2] = 4096;
    CACHE_SIZE[3] = 8192;
    CACHE_SIZE[4] = 16384;
    int ASSOCIATIVITY[4];
    ASSOCIATIVITY[0] = 1;
    ASSOCIATIVITY[1] = 2;
    ASSOCIATIVITY[2] = 4;
    ASSOCIATIVITY[3] = 8;

    // Direct mapping, Size 1024, FIFO
    cout << hex;
    int numCacheSets = (CACHE_SIZE[0] / BLOCK_ELEMS) / ASSOCIATIVITY[0];
    int indexBits = log2(numCacheSets);

    // Cache
    vector< list<block> > cache(numCacheSets);

    for (int i = 0; i < TOTAL; i++) {   // trace file
        long long index = addresses.at(i) >> BLOCK_BITS;
        index = index & BITMASK[3]; 
        long long tag = addresses.at(i) >> (indexBits + BLOCK_BITS);
    }
}

// Basic log_2 function. will fail on non-power-of-two
int log2(int num) {
    if (num <= 0) return -1;

    int log = 0;
    for (log = 0; num != 1; log++)
        num /= 2;

    return log;
}
