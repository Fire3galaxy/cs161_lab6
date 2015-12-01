#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main() {
    // Load all addresses into single vector first
    const long TOTAL = 1212176;
    vector<long long> addresses(TOTAL);

    ifstream fs;
    fs.open("trace");

    fs >> hex; // set up hex input
    for (int i = 0; fs >> addresses.at(i); i++);

    fs.close();

    // Get index and tag bits of addresses
    const int blockBits = 4;
    int indexBits = 10;

    //to get tag for different size
    const long long BITMASK[8];
    BITMASK[0] = 0x0000000000000007;
    BITMASK[1] = 0x000000000000000F;
    BITMASK[2] = 0x000000000000001F; 
    BITMASK[3] = 0x000000000000003F; // 6 bits (direct map 1024)
    BITMASK[4] = 0x000000000000007F; // 7 bits (direct map 2048)
    BITMASK[5] = 0x00000000000000FF; // 8 bits (direct map 4096)
    BITMASK[6] = 0x00000000000001FF; // 9 bits (direct map 8192)
    BITMASK[7] = 0x00000000000003FF; // 10 bits (direct map 16384)

    cout << hex; // set up hex output

    for (int i = 0; i < TOTAL; i++) {
        long long index = addresses.at(i) >> blockBits;
        index = index & BITMASK[3]; 

        //for each line, assign tags
        long long tag = addresses.at(i) >> (indexBits + blockBits);
        
        cout << index << ' ' << tag << endl;
        
    }
}
