
#define ASSERT(RES, EX, LEN)	\
for (int i = 0; i < LEN; i++) { \
	if (RES[i] != EX[i]) { \
		std::cout << "Error at " << i << std::endl; \
		return false; \
	} \
} \

#define ASSERT_32(RES, EX) ASSERT(RES, EX, 32)

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
#define TEST_DATA(ARR_NAME, BASE_INDEX) \
	ARR_NAME[BASE_INDEX + 0] = 8; \
	ARR_NAME[BASE_INDEX + 1] = data[2] = 0;\
	ARR_NAME[BASE_INDEX + 3] = 4 << 28;\
	ARR_NAME[BASE_INDEX + 4] = 0;\
	ARR_NAME[BASE_INDEX + 5] = 63 << 26;\
	ARR_NAME[BASE_INDEX + 6] = ONES;\
	ARR_NAME[BASE_INDEX + 7] = ONES >> 8;\

#include "compress.h"
#include "tests.h"
#include "const.h"
#include <stdlib.h>
#include <iostream>


bool divideIntoWordsTest()
{
	unsigned int data[] = { 1,2,3,4,5,6,7,8,9,10,
		11,12,13,14,15,16,17,18,19,20,
		21,22,23,24,25,26,27,28,29,30,
		31 };
	unsigned int* results = compress(data, 31);

	unsigned int expected[32];
	expected[0] = 0x7FFFFFFF & data[0];
	for (int i = 1; i < 32; i++){
		expected[i] = 0x7FFFFFFF & ((data[i] << i) | data[i - 1] >> (32 - i));
	}

	ASSERT_32(results, expected);

	std::cout << "Division into words succeeded" << std::endl;
	free(results);
	return true;
}



bool extendDataTest() {
	unsigned int data[31] = { 0 };
	data[0] = 88;
	data[3] = 4;
	data[1] = ONES31 | BIT31;
	data[2] = ONES31 | BIT31;
	data[4] = ONES31 | BIT31;

	unsigned int expected[31] = { 0 };
	expected[0] = 88;
	expected[3] = 4;
	expected[1] = ONES31 | BIT31;
	expected[2] = ONES31 | BIT31;
	expected[4] = ONES31 | BIT31;
	for (int i = 5; i < 32; i++) {
		expected[i] = BIT31;
	}

	unsigned int* res = compress(data, 31);

	ASSERT_32(res, expected)

	std::cout << "Extension succeeded" << std::endl;
	free(res);
	return true;

}

bool warpCompressionTest(){

	unsigned int data[31] = {0};
	TEST_DATA(data, 0);

//	data[0] = 8;
//	data[1] = data[2] = 0;
//	data[3] = 4 << 28;
//	data[4] = 0;
//	data[5] = 63 << 26;
//	data[6] = ONES;
//	data[7] = ONES >> 8;

	unsigned int expected[6] = {8, 3|BIT31, 4, 1|BIT31, 2|BIT3130, 24|BIT31};

	unsigned int* res = compress(data, 31);

	ASSERT(res, expected, 6);

	std::cout << "WarpCompression succeeded" << std::endl;
	free(res);
	return true;

}

bool blockCompressionTest(){
	unsigned int data[62] = {0};
	TEST_DATA(data, 0)
	TEST_DATA(data, 31)

	unsigned int * res = compress(data, 62);
	return true;
}