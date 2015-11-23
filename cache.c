//Cache Simulator demo 
//Created by Abhishek Mitra 
//Donot remove the headers 
//Usage gcc cache.c -o cache 
//Download and unzip trace file from: http://www.cs.ucr.edu/~amitra/cs161l/cache/trace.zip
//Execute the program:  ./cache trace 

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
//For a 32Bit processor 1 Word = 32bits
#define CACHE_SIZE 16384
#define CACHE_BLOCK_SIZE 16 // 1 Block = 4 Words = 4*32Bits
#define CACHE_BLOCKS CACHE_SIZE/CACHE_BLOCK_SIZE
//  ------------------------------------------------------------------------------------------------
// |				Tag		           |	Index 		        | Offset   |
// |31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00|
// |_______________________________________________________________________________________________|
// Case for a Direct Mapped Cache of size 16384 Bytes i.e. 1024 locations of 16 Bytes each
int main()
{
	int cache[CACHE_BLOCKS];
	char src[12]="0x";//store the hex address here
	FILE *trace;
	char address[9];//temp variable to read from text file
	char *diag;//diagnostic for strtoll
	//bash> man strtoll for clarifications
	unsigned int index=0;
	long long int test=0;
	int tag=0;
	long int miss=0, hit=0;
	trace = fopen("trace", "r"); //bash> man fopen
	printf("Cache Simulator Demo!");
	for(index=0; index<CACHE_BLOCKS; index++)
		cache[index]=0; //reset all the cache locations
	while (fscanf(trace, "%s", address)!=EOF){
		src[2]=0;//last character of a string is a NULL
		//address countains the current read address
		strcat(src, address); //bash> man strcat
		//printf("Checking %s \n", src);
		test = strtoll(address, &diag, 16);//String to longlong
		//printf(" Hex Equivalent is %x \n", test);
		//printf(" Diagnostic Val is %s \n", *diag);
		tag = test >> 14; //Obtain tag from the address[31..14]
		index =test >> 4; //address[31..4]
		index = index & 0x0000003FF;//address[13..4]
		//printf("Index = %d\n", index);
		//printf(" Tag = %x\n", tag);
		if (index<0) printf(" Index = %d!\n",index);//sanity check 1
		if (tag<0) printf(" Index = %d\n",tag);//sanity check 2
		if(cache[index]==tag){
		//	printf("Hit!\n");
			hit++;
		}
		if(cache[index]!=tag)
		{
		//	printf("Miss");
			cache[index]=tag;
			miss++;
		}

}
printf("Hit = %d ",hit);
printf("Miss = %d\n",miss);
printf("Miss Ratio  = %g",(miss/1012176.0)*100.0);
return 0;
}


