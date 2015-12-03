#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <list>

using namespace std;

int log_2(int num);
double simulate_cache(int cache, int asso, int rp, vector<long long>& addresses);
struct block {
    bool valid;
    long long tag;
    
    block() {
        valid = false;
        tag = 0;
    }

    block(bool v, long long l) {
        valid = v;
        tag = l;
    }
};

// Constants
const long TOTAL = 1012176; // file size
const int BLOCK_BITS = 4;   // block offset, 16 elements
const int BLOCK_ELEMS = 16; // for cache size / elem per block
long long BITMASK[8] = {    // to get tag for different size
    0x0000000000000007,
    0x000000000000000F,
    0x000000000000001F,
    0x000000000000003F,     // 6 bits (direct map 1024)
    0x000000000000007F,     // 7 bits (direct map 2048)
    0x00000000000000FF,     // 8 bits (direct map 4096)
    0x00000000000001FF,     // 9 bits (direct map 8192)
    0x00000000000003FF};    // 10 bits (direct map 16384)
int CACHE_SIZE[5] = {       // cache size = # of long long in cache
    1024,
    2048,
    4096,
    8192,
    16384};
int ASSOCIATIVITY[4] = {1,2,4,8};

int main() {
    // Load all addresses into single vector first
    vector<long long> addresses(TOTAL);

    ifstream fs;
    fs.open("trace");

    fs >> hex; // set up hex input
    for (int i = 0; i < TOTAL &&  fs >> addresses.at(i); i++);

    fs.close();

    cout << setprecision(2) << fixed;

    for(int r = 0; r < 2; r++) //for 0 = LRU, 1 = FIFO
    {
        if(r == 0)
            cout << "      LRU Replacement Policy" << endl;
        else
            cout << endl << "     FIFO Replacement Policy" << endl;

        for(int a = 0; a < 5; a++) //from fully to 8-ways
        {
            if(a > 0)
            {
                int tmp = a-1;
                cout << ASSOCIATIVITY[tmp] << "  ";
            }
            for(int s = 0; s < 5; s++) //from 1028 to 16384
            {
                if(a == 0)
                {
                    if(s == 0)
                        cout << "   ";
                    cout <<  dec << CACHE_SIZE[s] << "  ";
                }
                else
                {
                    int assoc = a-1;
                    cout << setfill('0') << setw(5) << simulate_cache(s, assoc, r, addresses) << " ";
                }
            }
            cout << endl;
        }
    }


}

double simulate_cache(int ch, int asso, int rp, vector<long long>& addresses)
{
    // Direct mapping, Size 1024, FIFO
    cout << hex;
    int numCacheSets = (CACHE_SIZE[ch] / BLOCK_ELEMS) / ASSOCIATIVITY[asso];
    int indexBits = log_2(numCacheSets);

    // Cache
    vector< list<block> > cache(numCacheSets);
    // Miss count
    long miss = 0;

    for (int i = 0; i < TOTAL; i++) {   // trace file
        long long index = addresses.at(i) >> BLOCK_BITS;
        index = index & BITMASK[ch + 3 - asso]; // 3-0, or 4-1, for bit masks based on size and associativity etc.
        long long tag = addresses.at(i) >> (indexBits + BLOCK_BITS);

        bool hit = false;
        for (list<block>::iterator it = cache.at(index).begin(); it != cache.at(index).end(); it++) {
            struct block b = *it;

            if (b.valid && b.tag == tag) {
                hit = true;
                if(rp == 0) //see it as LRU, 
                {
                   cache.at(index).erase(it);
                   cache.at(index).push_front(b);
                }
                break;
            }
        }

        if (!hit) {
            if (!(cache.at(index).size() < ASSOCIATIVITY[asso])) // pop_back oldest/least recently used
                cache.at(index).pop_back();

            struct block insert(true, tag);
            cache.at(index).push_front(insert); // Place new "data" in the cache
            
            miss++;
        }
    }

    return ((double) miss) / ((double) TOTAL) * 100;
}

// Basic log_2 function. will fail on non-power-of-two
int log_2(int num) {
    if (num <= 0) return -1;

    int log = 0;
    for (log = 0; num != 1; log++)
        num /= 2;

    return log;
}
