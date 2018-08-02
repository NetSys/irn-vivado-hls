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

struct axiWordAck
{
   //My format for ack: QID (24), opcode (5), syndrome (1), ackNo (24), msnNo (24), sackNo (24) 
   ap_uint<128>  data;
 //  ap_uint<8>   strb;
 //  ap_uint<1>   last;
};

struct QPInfo 
{
  ap_uint<24>  nextSNtoSend;
  ap_uint<24>  lastAckedPsn;
  ap_uint<24>  retransmitSN;
  ap_uint<24>  recoverSN;
  ap_uint<24>  rate_factor;
  ap_uint<BDP>  sack_bitmap;
  bool doRetransmit;
  bool inRecovery;
	bool quickTimeout;
	bool findNewHole;
};

struct QPInfoSub1
{
  ap_uint<24>  nextSNtoSend;
  ap_uint<24>  lastAckedPsn;
  ap_uint<24>  retransmitSN;
  ap_uint<24>  recoverSN;
  ap_uint<BDP>  sack_bitmap;
  bool doRetransmit;
  bool inRecovery;
	bool findNewHole;
};

struct QPInfoSub2
{
  ap_uint<24>  nextSNtoSend;
  ap_uint<24>  lastAckedPsn;
  ap_uint<24>  retransmitSN;
  bool doRetransmit;
	bool findNewHole;
	bool quickTimeout;
  ap_uint<24>  rate_factor;
};



struct AckInfo
{
	bool ackSyndrome;
	ap_uint<24> cumAck;
	ap_uint<24> sackNo;
};



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

