/*
 * decompress.h
 *
 *  Created on: May 23, 2018
 *      Author: suchanj
 */

#ifndef DECOMPRESS_H_
#define DECOMPRESS_H_

unsigned int* decompress(
		unsigned int* data,
		unsigned int dataSize,
		unsigned int* outSize,
		float* pTransferToDeviceTime,
		float* pCompressionTime,
		float* ptranserFromDeviceTime);



#endif /* DECOMPRESS_H_ */
