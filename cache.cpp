#include <iostream>
#include <vector>
#include <cstdio>

#define  BLOCK_SIZE     16
#define  CACHE_SIZE     16384

int main () {
   // build cache
   int number_of_blocks = CACHE_SIZE / BLOCK_SIZE;
   std::vector<unsigned long long> cache;

   for (int i = 0; i < number_of_blocks; ++i)
      cache.push_back(0);

   // read memory accesses from standard input
   unsigned long long read_line;
   int total = 0, miss = 0;
   while (std::cin >> std::hex >> read_line) {
      total++;
      unsigned long long tag   = read_line >> (10 + 4); // get tag
      unsigned long long index = read_line >> 4;        // get index
      index = index & 0x00000000000003FF;

      // check for miss
      if (cache[index] != tag) {
         cache[index] = tag;
         miss++;
      }
   }
   double miss_rate = (double)miss / (double)(total);

   printf("total accesses: %d\n", total);
   printf("miss rate:      %0.2lf\n", miss_rate * 100.0);

   return 0;
}

