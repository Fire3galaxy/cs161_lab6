#include <iostream>
#include <fstream>

using namespace std;

int main() {
    ifstream fs;
    fs.open("trace");

    long long c = 0;
    long long address = 0;

    fs >> hex;
    while (fs >> c)
        cout << c << '\n';

    fs.close();
}
