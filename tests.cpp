
#include "tests.h"
#include "const.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include "RegularPackage.h"

/*
 * 1. 0 | 8
 * 2. 00 | 30x0
 * 3. 000 | 29x0
 * 4. 0100(4) | 28x0
 * 5. 5x0 | 27x0
 * 6. 6x1 | 26x0
 * 7. 7x1 | 25x1
 * 8. 8x0 | 24x1
 * 9..31 0...
 */
void generateTestData(unsigned int* arr, int baseIndex){
	arr[baseIndex + 0] = 8;
	arr[baseIndex + 1] = 0;
	arr[baseIndex + 3] = 4 << 28;
	arr[baseIndex + 4] = 0;
	arr[baseIndex + 5] = 63 << 26;
	arr[baseIndex + 6] = ONES;
	arr[baseIndex + 7] = ONES >> 8;
}

void generateWanderingTestData(unsigned int* arr, int baseIndex){
	arr[baseIndex] = 1;
	arr[baseIndex + 31] = 1 <<31;
	for(int i = 0; i < 30; i++){
		arr[baseIndex+ 31 + (i+1) * 32] = 1 << 30 - i;
	}
}


void generateRandomData(unsigned int* tab, unsigned int size, unsigned int everyN) {
	int res;
	int treshold=RAND_MAX/everyN;
	srand(1337);
	for (long long int i=0;i<size*32;i++) {
		long long int word=i>>5; // /32
		int off=i&31; // %32
		if (off==0) {
			//tab[word]=0;
			res=0;
			if (word%1000==0) printf("%f%%   \r",i*100.0/(double)(size*32-1));
			//if (word>0) printf("%x \n",tab[word-1]);
		}
		//double v=(double)rand()/(double)RAND_MAX;
		int bit=(rand()<treshold)<<off;
		res=res|bit;
		if (off==31) {
			tab[word]=res;
			//printf("%x \n",res);
		}
	}
	printf("100%%                        \n");
}

void generateWanderingExpectedData(unsigned int* expected, int baseIndex){
	expected[baseIndex] = 1;
	expected[baseIndex + 1] = BIT31 | 31;
	for(int i=0; i < 30; i++){
		expected[baseIndex + 2+3*i] = BIT31 | i + 1;
		expected[baseIndex + 2+3*i + 1] = 1;
		expected[baseIndex + 2+3*i + 2] = BIT31 | 30 - i;
	}
	expected[baseIndex + 91] = BIT31 | 32;
	expected[baseIndex + 92] = 1;

}

void initializeTestData(int baseIndex, unsigned int* arr){
	generateTestData(arr, baseIndex);
}

//bool divideIntoWordsTest()
//{
//	unsigned int data[] = { 1,2,3,4,5,6,7,8,9,10,
//		11,12,13,14,15,16,17,18,19,20,
//		21,22,23,24,25,26,27,28,29,30,
//		31 };
//
//	unsigned long long int compressedSize;
//	unsigned int* results = compress(data, 31, &compressedSize, NULL, NULL, NULL);
//
//	unsigned int expected[32];
//	expected[0] = 0x7FFFFFFF & data[0];
//	for (int i = 1; i < 32; i++){
//		expected[i] = 0x7FFFFFFF & ((data[i] << i) | data[i - 1] >> (32 - i));
//	}
//
//	ASSERT_32(results, expected);
//
//	std::cout << "Division into words succeeded" << std::endl;
//	free(results);
//	return true;
//}
//
//bool extendDataTest() {
//	unsigned int data[31] = { 0 };
//	data[0] = 88;
//	data[3] = 4;
//	data[1] = ONES31 | BIT31;
//	data[2] = ONES31 | BIT31;
//	data[4] = ONES31 | BIT31;
//
//	unsigned int expected[31] = { 0 };
//	expected[0] = 88;
//	expected[3] = 4;
//	expected[1] = ONES31 | BIT31;
//	expected[2] = ONES31 | BIT31;
//	expected[4] = ONES31 | BIT31;
//	for (int i = 5; i < 32; i++) {
//		expected[i] = BIT31;
//	}
//	unsigned long long  int compressedSize;
//	unsigned int* res = compress(data, 31, &compressedSize, NULL, NULL, NULL);
//
//	ASSERT_32(res, expected)
//
//	std::cout << "Extension succeeded" << std::endl;
//	free(res);
//	return true;
//
//}
//
//TEST_DEC(warpCompressionTest)
//	unsigned int data[31] = {0};
//	generateTestData(data, 0);
//
////	data[0] = 8;
////	data[1] = data[2] = 0;
////	data[3] = 4 << 28;
////	data[4] = 0;
////	data[5] = 63 << 26;
////	data[6] = ONES;
////	data[7] = ONES >> 8;
//
//	unsigned int expected[6] = {8, 3|BIT31, 4, 1|BIT31, 2|BIT3130, 24|BIT31};
//	unsigned long long  int compressedSize;
//	unsigned int* res = compress(data, 31, &compressedSize, NULL, NULL, NULL);
//
//	ASSERT(res, expected, 6);
//
//TEST_END
//
//TEST_DEC(blockCompressionTest)
//	unsigned int data[32*31] = {0};
//	for(int i = 0; i<32; i++){
//		initializeTestData(i*31, data);
//	}
//	unsigned long long int compressedSize;
//	unsigned int * res = compress(data, 31*32, &compressedSize, NULL, NULL, NULL);
//
//	unsigned int help[6] = {8, 3|BIT31, 4, 1|BIT31, 2|BIT3130, 24|BIT31};
//	ASSERT_MODULO(res, help, 6*32, 6);
//TEST_END
//
//TEST_DEC(blockMergeTest)
//	unsigned int data[32*31] = {0};
//	unsigned long long int compressedSize;
//	unsigned int expected[1] = {BIT31 | 1024};
//	unsigned int* res = compress(data, 31*32, &compressedSize, NULL, NULL, NULL);
//	ASSERT(res, expected, 1);
//TEST_END
//
//TEST_DEC(blockMergeWithOnesStartsTest)
//	unsigned int data[32*31] = {0};
//
//	for(int i = 0; i < 32; i+=2){
//			data[31*i] = ONES;
//
//	}
//	unsigned long long int compressedSize;
//	unsigned int* res = compress(data, 31*32, &compressedSize, NULL, NULL, NULL);
//	unsigned int help[] = {BIT3130 | 1, 1, BIT31 | 62 };
//	ASSERT_MODULO(res, help, 3*16,3)
//TEST_END
//
//TEST_DEC(blockMergeAlternatingTest)
//	unsigned int data[32*31] = {0};
//
//	for(int i = 2; i < 32; i+=4){
//		for(int j = 0; j < 62; j++){
//			data[31*i+j] = ONES;
//		}
//	}
//	unsigned long long int compressedSize;
//	unsigned int* res = compress(data, 31*32, &compressedSize, NULL, NULL, NULL);
//	unsigned int expected[] = {BIT31 | 64, BIT3130 | 64};
//	ASSERT_MODULO(res, expected, 16, 2);
//TEST_END
//
//TEST_DEC(blockMergeFinalLiterals)
//	unsigned int data[31*32] = {0};
//
//	for(int i = 0; i < 32; i++){
//		data[31*(i+1) - 1] = 88;
//	}
//	unsigned long long int compressedSize;
//	unsigned int* res = compress(data, 31*32, &compressedSize, NULL, NULL, NULL);
//	unsigned int expected[] = {BIT31 | 31, 44};
//	ASSERT_MODULO(res, expected, 64, 2);
//TEST_END
//
//TEST_DEC(blockMergeWanderingLiterals)
//	unsigned int data[31*32] = {0};
//
//	generateWanderingTestData(data,0);
//	unsigned long long int compressedSize;
//	unsigned int* res = compress(data, 31*32, &compressedSize, NULL, NULL, NULL);
//	unsigned int expected[93];
//
//	generateWanderingExpectedData(expected, 0);
//
//	ASSERT(res, expected, 93)
//
//TEST_END
//
//TEST_DEC(multiBlockTest)
//	unsigned int data[2*31*32] = {0};
//	generateWanderingTestData(data, 0);
//	generateWanderingTestData(data, 31*32);
//	unsigned long long int compressedSize;
//	unsigned int* res = compress(data, 2*31*32, &compressedSize, NULL, NULL, NULL);
//	unsigned int expected[93*2];
//	generateWanderingExpectedData(expected, 0);
//	generateWanderingExpectedData(expected, 93);
//
//	ASSERT(res, expected, 186);
//
//TEST_END

bool compressAndDecompressTest(PackageBase* package){
	std::cout << compressAndDecompressTest << ": ";
	float c_transferToDevice, c_transferFromDevice, c_compression, d_transferToDevice, d_transferFromDevice, d_compression;
	int blocks = 1024*64;
	int size = 31*32*blocks;
	unsigned int* data = (unsigned int*)malloc(sizeof(int)*size);
	for(int j = 0; j < blocks; j++){
		generateWanderingTestData(data, j*31*32);
	}
	//RegularPackage<T> package = RegularPackage<T>();
	//T compressedSize, decompressedSize;
	package->compressData(data, size);
	//unsigned int* res = compress<T>(data, size, &compressedSize, &c_transferToDevice, &c_compression, &c_transferFromDevice);
	package->decompressData();
	//unsigned int* decomp = decompress<T>(res, compressedSize, &decompressedSize, &d_transferToDevice, &d_compression, &d_transferFromDevice);
	package->performAssert();
	//printf("original size: %d, decompressed size: %d");
	//if(decompressedSize != size){
	//	printf("decompressed size does not match");
	//	return false;
	//}
	//ASSERT(decomp, data, decompressedSize)
	free(data);

	PackageBase::Times t = package->getTimes();
	printf("Compression \n");
	printf("Transfer to device: %f \n", t.c_transferToDevice);
	printf("Compression: %f \n", t.c_compression);
	printf("Transfer from device: %f \n", t.c_transferFromDevice);

	printf("Decompression \n");
	printf("Transfer to device: %f \n", t.d_transferToDevice);
	printf("Compression: %f \n", t.d_compression);
	printf("Transfer from device: %f \n", t.d_transferFromDevice);
TEST_END

//TEST_DEC(zerosTest)
//	float c_transferToDevice, c_transferFromDevice, c_compression, d_transferToDevice, d_transferFromDevice, d_compression;
//	int blocks = 1024;
//	int size = 31*32*blocks; //16MB of ints
//	unsigned int* data = (unsigned int*)malloc(sizeof(int) * size);
//	std::memset(data, 0, size);
////	std::ofstream outFile;
////	outFile.open("randomDataTest", std::ios::out | std::ios::binary);
////	outFile.write((char*)data, sizeof(int)*size);
////	outFile.close();
//	unsigned long long int compressedSize, decompressedSize;
//	unsigned int* res = compress(data, size, &compressedSize, &c_transferToDevice, &c_compression, &c_transferFromDevice);
//	unsigned int* decomp = decompress(res, compressedSize, &decompressedSize, &d_transferToDevice, &d_compression, &d_transferFromDevice);
//	ASSERT(decomp, data, decompressedSize)
//	free(decomp);
//TEST_END
//

//template<class T>
//TEST_DEC(randomDataTest)
//	float c_transferToDevice, c_transferFromDevice, c_compression, d_transferToDevice, d_transferFromDevice, d_compression;
//	int blocks = 1024*128;
//	int size = 31*32*blocks; //16MB of ints
//	unsigned int* data = (unsigned int*)malloc(sizeof(int) * size);
//	generateRandomData(data, size, (1 << 4));
////	std::ofstream outFile;
////	outFile.open("randomDataTest", std::ios::out | std::ios::binary);
////	outFile.write((char*)data, sizeof(int)*size);
////	outFile.close();
//	T compressedSize, decompressedSize;
//	unsigned int* res = compress<T>(data, size, &compressedSize, &c_transferToDevice, &c_compression, &c_transferFromDevice);
//	unsigned int* decomp = decompress<T>(res, compressedSize, &decompressedSize, &d_transferToDevice, &d_compression, &d_transferFromDevice);
//	std::cout << size << std::endl;
//	std::cout << compressedSize << std::endl;
//	std::cout << decompressedSize <<std::endl;
//	ASSERT(decomp, data, size)
//	//free(decomp);
//TEST_END

//template bool randomDataTest<unsigned long long int>();
//template bool randomDataTest<unsigned int>();
