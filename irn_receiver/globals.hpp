/* Author: Radhika Mittal
 * File description: header file for defining structs and key modules. 
 */ 
#ifndef GLOBALS_HPP_
#define GLOBALS_HPP_

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <stdint.h>
#include <cstdlib>

#include <hls_stream.h>
#include <ap_int.h>

#include "bitmapSize.hpp"

using namespace hls;

#define N 1000 

//opcode definitions as per Sec 9.2.1 of RDMA specification by Infiniband Trade Association
#define SEND_LAST 2
#define SEND_LAST_IMM 3
#define SEND_ONLY 4
#define SEND_ONLY_IMM 5
#define WRITE_LAST 8
#define WRITE_ONLY 10
#define READ_REQ 12
#define WRITE_LAST_IMM 9
#define WRITE_ONLY_IMM 11

//struct for metadata in data packets.
struct MetaData
{
	ap_uint<5> opcode;
	ap_uint<24> seqNo;
};

//struct to store output of receiveAck module. 
struct AckInfo
{
	bool ackSyndrome;
	ap_uint<24> cumAck;
	ap_uint<24> curMSN;
	ap_uint<24> sackNo;
	ap_uint<LOGBDP> numCQEDone;
};

//struct to store relevant QP context. 
struct QPInfo 
{
	ap_uint<24>	expectedSeq;
	ap_uint<24>	curMSN;
	ap_uint<BDP>	ooo_bitmap1;
	ap_uint<BDP>	ooo_bitmap2;
};

//defining the receiveData module.
void receiveData(
			stream<MetaData> &arrivedMetaData,
			stream<QPInfo> &perQPInfoIn,
			stream<QPInfo> &perQPInfoOut,
			stream<AckInfo> &newAckInfo
			);

#endif // GLOBALS_H_ not defined

