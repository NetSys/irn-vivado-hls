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

struct axiWordData
{
   //My format for data: QID (24), opcode (5), seqNo (24) 
   ap_uint<128>  data; 
//   ap_uint<8>   strb;
//   ap_uint<1>   last;
};

struct axiWordAck
{
   //My format for ack: QID (24), opcode (5), syndrome (1), ackNo (24), msnNo (24), sackNo (24) 
   ap_uint<128>  data;
 //  ap_uint<8>   strb;
 //  ap_uint<1>   last;
};

struct MetaData
{
	ap_uint<5> opcode;
	ap_uint<24> seqNo;
};

struct AckInfo
{
	bool ackSyndrome;
	ap_uint<24> cumAck;
	ap_uint<24> curMSN;
	ap_uint<24> sackNo;
	ap_uint<LOGBDP> numCQEDone;
};


struct QPInfo 
{
  ap_uint<24>  expectedSeq;
  ap_uint<24>  curMSN;
  ap_uint<BDP>  ooo_bitmap1;
  ap_uint<BDP>  ooo_bitmap2;
};

/*struct OtherInfo 
{
  ap_uint<BDP>  ones;
};*/




void receiveData(
			stream<MetaData> &arrivedMetaData,
      stream<QPInfo> &perQPInfoIn,
      stream<QPInfo> &perQPInfoOut,
			stream<AckInfo> &newAckInfo
      );

#endif // GLOBALS_H_ not defined

