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

//struct to store relevant QP context (per-flow state).
struct QPInfo 
{
	ap_uint<24>	nextSNtoSend;
	ap_uint<24>	lastAckedPsn;
	ap_uint<24>	retransmitSN;
	ap_uint<24>	recoverSN;
	ap_uint<24>	rate_factor;
	ap_uint<BDP>	sack_bitmap;
	bool doRetransmit;
	bool inRecovery;
	bool quickTimeout;
	bool findNewHole;
};

//struct to store a subset of relevant QP context.
struct QPInfoSub1
{
	ap_uint<24>	nextSNtoSend;
	ap_uint<24>	lastAckedPsn;
	ap_uint<24>	retransmitSN;
	ap_uint<24>	recoverSN;
	ap_uint<BDP>	sack_bitmap;
	bool doRetransmit;
	bool inRecovery;
	bool findNewHole;
};

//struct to store another subset of relevant QP context.
struct QPInfoSub2
{
	ap_uint<24>	nextSNtoSend;
	ap_uint<24>	lastAckedPsn;
	ap_uint<24>	retransmitSN;
	bool doRetransmit;
	bool findNewHole;
	bool quickTimeout;
	ap_uint<24>	rate_factor;
};



//struct to store relevant information carried in an ack.
struct AckInfo
{
	bool ackSyndrome;
	ap_uint<24> cumAck;
	ap_uint<24> sackNo;
};


//defining the three key sender side modules, which are synthesized.
void txFree(
			stream<QPInfoSub1> &perQPInfoIn,
			stream<QPInfoSub1> &perQPInfoOut,
			stream<bool> &sendAllowedStream,
			stream< ap_uint<24> > &toSendStream
			);

void receiveAck(
			stream<AckInfo> &newAckInfo,	
			stream<QPInfoSub1> &perQPInfoIn,
			stream<QPInfoSub1> &perQPInfoOut
		 );

void timeout(
			stream<QPInfoSub2> &perQPInfoIn,
			stream<QPInfoSub2> &perQPInfoOut,
			stream<bool> &timeoutExtend 
			);



#endif // GLOBALS_H_ not defined

