///*
// * NoSortPackage.cpp
// *
// *  Created on: Feb 13, 2019
// *      Author: holgus103
// */
//
//#include "NoSortPackage.h"
//#include "compress.h"
//#include "kernels.h"
//#include "cuda_runtime.h"
//#include "device_launch_parameters.h"
//#include <math.h>
//#include <time.h>
//#include <stdlib.h>
//#include <thrust/remove.h>
//#include <thrust/device_ptr.h>
//#include "timeMeasuring.h"
//
//NoSortPackage::NoSortPackage() {
//
//}
//
//NoSortPackage::~NoSortPackage() {
//
//}
//
//void  NoSortPackage::compressData(unsigned int* data, unsigned long long int size){
//	// times to be measured
//		float transferToDeviceTime;
//		float compressionTime;
//		float transferFromDeviceTime;
//
//		// start measuring time
//		cudaEvent_t start, stop;
//		cudaEventCreate(&start);
//		cudaEventRecord(start,0);
//
//		unsigned long long blockCount = dataSize / (31*32);
//
//		if(dataSize % (31*32)> 0){
//			blockCount++;
//		}
//		// assign blockCount
//		this->orderingLength = blockCount;
//
//		unsigned int *data_gpu, *compressed_gpu;
//		unsigned long long int *blockCounts_gpu, *sizeCounter_gpu, *orderArray_gpu;
//
//		// calculate max output size (one extra bit for every 31 bits)
//		unsigned long long int maxExpectedSize = 8*sizeof(int)*dataSize;
//		if(maxExpectedSize % 31 > 0){
//			maxExpectedSize /= 31;
//			maxExpectedSize++;
//		}
//		else{
//			maxExpectedSize /= 31;
//		}
//
//		dim3 blockSize = dim3(32, 32, 1);
//
//		// allocate memory on the device
//		if(cudaSuccess != cudaMalloc((void**)&sizeCounter_gpu, sizeof(unsigned long long int))){
//			std::cout << "Could not allocate space for size counter" << std::endl;
//			FREE_ALL
//			return NULL;
//		}
//		if(cudaSuccess != cudaMalloc((void**)&orderArray_gpu, blockCount * sizeof(unsigned long long int))){
//			std::cout << "Could not allocate space for order array" << std::endl;
//			FREE_ALL
//			return NULL;
//		}
//		if(cudaSuccess != cudaMalloc((void**)&data_gpu, dataSize * sizeof(int))){
//			std::cout << "Could not allocate space for the data" << std::endl;
//			FREE_ALL
//			return NULL;
//		}
//		if(cudaSuccess != cudaMalloc((void**)&compressed_gpu, maxExpectedSize * sizeof(int))){
//			std::cout << "Could not allocate space for the compressed output" << std::endl;
//			FREE_ALL
//			return NULL;
//		}
//		if(cudaSuccess != cudaMalloc((void**)&blockCounts_gpu, blockCount* sizeof(unsigned long long int))){
//			std::cout << "Could not allocate space for the block sizes" << std::endl;
//			FREE_ALL
//			return NULL;
//		}
//
//		// copy input
//		if(cudaSuccess != cudaMemcpy(data_gpu, data_cpu, dataSize*sizeof(int), cudaMemcpyHostToDevice)){
//			std::cout << "Could not copy input" << std::endl;
//			FREE_ALL
//			return NULL;
//		}
//
//		// get transfer time
//		cudaEventCreate(&stop);
//		cudaEventRecord(stop,0);
//		cudaEventSynchronize(stop);
//		cudaEventElapsedTime(&transferToDeviceTime, start,stop);
//
//		// restart time measuring
//		cudaEventCreate(&start);
//		cudaEventRecord(start,0);
//
//		// call compression kernel
//		compressData<<<blockCount,blockSize>>>(data_gpu, compressed_gpu, blockCounts_gpu, orderArray_gpu, sizeCounter_gpu, dataSize);
//
//		// remove unnecessary data
//		cudaFree((void*)data_gpu);
//
//		// allocate memory for block sizes
//		this->blockSizes = (unsigned long long int*)malloc(sizeof(unsigned long long int) *blockCount);
//
//		// copy block sizes
//		if(cudaSuccess != cudaMemcpy(this->blockSizes, blockCounts_gpu, blockCount * sizeof(unsigned long long int), cudaMemcpyDeviceToHost)){
//			std::cout << "Could not copy last block counts" << std::endl;
//			FREE_ALL
//			return NULL;
//		}
//
//		// allocate ordering array
//		this->orderingArray = (unsigned long long int*) malloc(sizeof(unsigned long long int) * blockCount);
//
//		// copy ordering array
//		if(cudaSuccess != cudaMemcpy(this->orderingArray, orderArray_gpu, blockCount * sizeof(unsigned long long int), cudaMemcpyDeviceToHost)){
//			std::cout << "Could not copy ordering array" << std::endl;
//			FREE_ALL
//			return NULL;
//		}
//
//		unsigned long long int outputSize = 0;;
//
//		if(cudaSuccess != cudaMemcpy(&outputSize, sizeCounter_gpu, sizeof(unsigned long long int), cudaMemcpyDeviceToHost)){
//			std::cout << "Could not copy last block offset" << std::endl;
//			FREE_ALL
//			return NULL;
//		}
//
//		SAFE_ASSIGN(outSize, outputSize)
//
//		// get compression time
//		cudaEventCreate(&stop);
//		cudaEventRecord(stop,0);
//		cudaEventSynchronize(stop);
//		cudaEventElapsedTime(&compressionTime, start,stop);
//
//		// restart time measuring
//		cudaEventCreate(&start);
//		cudaEventRecord(start,0);
//
//		// allocate memory for results
//		unsigned int* compressed_cpu = (unsigned int*)malloc(sizeof(int)* outputSize);
//		// copy compressed data
//		if(cudaSuccess != cudaMemcpy((void*)compressed_cpu, (void*)compressed_gpu, outputSize * sizeof(int), cudaMemcpyDeviceToHost)){
//			std::cout << "Could not copy final output" << std::endl;
//		}
//
//		// free gpu memory
//		cudaFree((void*)compressed_gpu);
//		cudaFree((void*)blockCounts_gpu);
//		cudaFree((void*)orderArray_gpu);
//
//		// get transfer time
//		cudaEventCreate(&stop);
//		cudaEventRecord(stop,0);
//		cudaEventSynchronize(stop);
//		cudaEventElapsedTime(&transferFromDeviceTime, start,stop);
//
//		// write results to pointers if specified
//		if(pCompressionTime != NULL) (*pCompressionTime) = compressionTime;
//		if(pTransferToDeviceTime != NULL) (*pTransferToDeviceTime) = transferToDeviceTime;
//		if(ptranserFromDeviceTime != NULL) (*ptranserFromDeviceTime) = transferFromDeviceTime;
//	return compressed_cpu;
//}
//
//unsigned int* NoSortPackage::decompressData(){
//	// times to be measured
//		float transferToDeviceTime;
//		float compressionTime;
//		float transferFromDeviceTime;
//
//		// start measuring time
//		CREATE_TIMER
//		START_TIMER
//
//		unsigned int *data_gpu, *result_gpu, *finalOutput_gpu, *output_cpu;
//		unsigned long long int* counts_gpu;
//		unsigned long long int blockCount = dataSize / 1024;
//
//		if(dataSize % 1024 > 0){
//			blockCount++;
//		}
//		cudaMalloc((void**)&data_gpu, sizeof(int)*dataSize);
//		cudaMalloc((void**)&counts_gpu, sizeof(unsigned long long int)*dataSize);
//		cudaMemcpy(data_gpu, data, sizeof(int)*dataSize, cudaMemcpyHostToDevice);
//
//		STOP_TIMER
//		GET_RESULT(transferToDeviceTime)
//		START_TIMER
//	//	counts_cpu = (unsigned int*) malloc(sizeof(int)*dataSize);
//		dim3 blockDim(32, 32);
//		// get blocked sizes
//		getCounts<<<blockCount,blockDim>>>(data_gpu, counts_gpu, dataSize);
//		unsigned long long int lastBlockSize;
//		cudaMemcpy(&lastBlockSize, counts_gpu  + (dataSize - 1), sizeof(unsigned long long int), cudaMemcpyDeviceToHost);
//		// scan block sizes
//		thrust::device_ptr<unsigned long long int> countsPtr(counts_gpu);
//		// get counts
//		thrust::exclusive_scan(countsPtr, countsPtr + dataSize, countsPtr);
//		unsigned long long int lastOffset;
//	//	thrust::inclusive_scan(counts_cpu, counts_cpu + dataSize, counts_cpu);
//		cudaMemcpy(&lastOffset, counts_gpu + (dataSize - 1), sizeof(unsigned long long int), cudaMemcpyDeviceToHost);
//		unsigned long long int outputSize = lastBlockSize + lastOffset;
//		unsigned long long int realSize = 31*outputSize;
//
//		if(realSize % 32 > 0){
//			realSize /=32;
//			realSize++;
//		}
//		else{
//			realSize /=32;
//		}
//		SAFE_ASSIGN(outSize, realSize);
//	//	free(counts_cpu);
//		cudaMalloc((void**)&result_gpu, sizeof(int) * outputSize);
//
//		decompressWords<<<blockCount,blockDim>>>(data_gpu, counts_gpu, result_gpu, dataSize);
//		cudaFree(data_gpu);
//		cudaFree(counts_gpu);
//
//		blockCount = outputSize / 1024;
//		if(dataSize % 1024 > 0){
//			blockCount++;
//		}
//
//		cudaMalloc((void**)&finalOutput_gpu, sizeof(int)*outputSize);
//		mergeWords<<<blockCount,blockDim>>>(result_gpu, finalOutput_gpu, outputSize);
//		cudaFree(result_gpu);
//
//		STOP_TIMER
//		GET_RESULT(compressionTime)
//		START_TIMER
//
//		output_cpu = (unsigned int*)malloc(sizeof(int) * outputSize);
//		cudaMemcpy(output_cpu, finalOutput_gpu, sizeof(int) * outputSize, cudaMemcpyDeviceToHost);
//		cudaFree(finalOutput_gpu);
//		STOP_TIMER
//		GET_RESULT(transferFromDeviceTime)
//
//		SAFE_ASSIGN(pCompressionTime, compressionTime);
//		SAFE_ASSIGN(pTransferToDeviceTime, transferToDeviceTime);
//		SAFE_ASSIGN(ptranserFromDeviceTime, transferFromDeviceTime);
//
//	return output_cpu;
//}
