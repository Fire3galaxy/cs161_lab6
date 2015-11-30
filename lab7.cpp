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

    fs >> hex;
    for (int i = 0; fs >> addresses.at(i); i++);

    fs.close();

    // Get index and tag bits of addresses
    for (int i = 0; i < TOTAL; i++) {
    }
}
